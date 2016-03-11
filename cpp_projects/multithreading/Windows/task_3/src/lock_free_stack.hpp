#ifndef LOCK_FREE_STACK_HPP
#define LOCK_FREE_STACK_HPP

#include <memory>

template <typename T>
class lock_free_stack
{
    struct Node
    {
        std::shared_ptr<T> item;
        Node* next;
        Node(T const& _item, Node* _next) : item(std::make_shared<T>(_item)), next(_next) {}
    };

    std::atomic<Node*>  head;
    std::atomic<Node*>  to_be_deleted;
    std::atomic<unsigned int>  threads_in_pop;
    void try_reclaim(Node* old_head);
    void chain_pending_nodes(Node* nodes);
    void chain_pending_nodes(Node* first, Node* last);
    void chain_pending_node(Node* n);

public:

    typedef T   value_type;

    lock_free_stack() : head() {}
    ~lock_free_stack() { while (!isEmpty()) pop(); }
    bool isEmpty() const { return nullptr == head; }
    T& front();
    T const& front() const;
    void push(T const& );
    std::shared_ptr<T> pop();
};

template <typename T>
T& lock_free_stack<T>::front()
{
    return *(head.load(std::memory_order_seq_cst))->item;
}

template <typename T>
T const& lock_free_stack<T>::front() const
{
    return *(head.load(std::memory_order_seq_cst))->item;
}

template <typename T>
void lock_free_stack<T>::push(T const& item)
{
    Node* newNode = new Node { item, head.load() };
    while(!head.compare_exchange_weak(newNode->next, newNode)) {}
}

template <typename T>
std::shared_ptr<T> lock_free_stack<T>::pop()
{
    ++threads_in_pop;

    //получаем указатель на текущий head
    Node* old_head = head.load(std::memory_order_seq_cst);

    //пока old_head не нулевой (стек пустой) && head перенаправлен на следующий Node (compare_exchange_weak) при условии, что на момент обмена head  не изменился
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next)) {}

    //формируем возвращаемое значение:
    std::shared_ptr<T> res; //данное по умолчанию
    if (old_head)
    {
        res.swap(old_head->item); //не копируем указатель, а обмениваем данные
    }

    try_reclaim(old_head); //вызываем вспомогательный метод

    return res;
}

template <typename T>
void lock_free_stack<T>::try_reclaim(Node* old_head)
{
    if (threads_in_pop == 1) //единственный в pop() => попытка удалить
    {
        //можно безопасно удалять  только что исключенный из списка old_head
        delete old_head;
        //Пытаемся удалить накопившиеся исключенные узлы
        Node* nodes_to_delete = to_be_deleted.exchange(nullptr); //захватить список на удаление
        if (!--threads_in_pop)//точно единственный?
        {
        //удаляем узлы, накопившиеся в списке  nodes_to_delete
            while(nodes_to_delete)
            {
                Node* next = nodes_to_delete->next;
                delete nodes_to_delete;
                nodes_to_delete = next;
            }
        }
        else if(nodes_to_delete)
        {
            chain_pending_nodes(nodes_to_delete);
        } //если в захваченном списке что-то было, вернуть это в общий список узлов на удаление
    }
    else
    {   //удалять old_head сейчас нельзя => добавим в список для удаления (удалим как-нибудь потом)
        chain_pending_node(old_head);
        --threads_in_pop;
    }
}

template <typename T>
void lock_free_stack<T>::chain_pending_nodes(Node* nodes)
{
    Node* last = nodes;
    while(Node* next = last->next) {last = next;} //доходим до конца списка
    chain_pending_nodes(nodes, last);
}

template <typename T>
void lock_free_stack<T>::chain_pending_nodes(Node* first, Node* last)
{
    last->next = to_be_deleted;
    while(!to_be_deleted.compare_exchange_weak(last->next, first));
}

template <typename T>
void lock_free_stack<T>::chain_pending_node(Node* n)
{
    chain_pending_nodes(n,n);
}



#endif // LOCK_FREE_STACK_HPP
