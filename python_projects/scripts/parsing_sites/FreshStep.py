#!/usr/bin/python

import os
import urllib2
import xlsxwriter
from bs4 import BeautifulSoup

PETSHOP_FRESHSTEP_URL = "http://www.petshop.ru/catalog/cats/tualety/vpityvayuschiy_napolnitel/troynaya_zaschita_vpityvayuschiy_napolnitel_1661/"
ZOO812_FRESHSTEP_URL = "http://zoo812.ru/products/fresh-step-trojnaya-zaschita-ot-zapaha-vpityvayuschij-napolnitel"
KORMKOM_FRESHSTEP_URL = "http://www.korkom.ru/p/1955/FRESH_STEP_Trojnaya_zazchita"
HAPPYPET_FRESHSTEP_URL = "http://xn----7sbgnmcfnotjrboh1ec2f.xn--p1ai/%D0%BA%D0%B0%D1%82%D0%B0%D0%BB%D0%BE%D0%B3/%D0%B4%D0%BB%D1%8F-%D0%BA%D0%BE%D1%88%D0%B5%D0%BA/%D0%BD%D0%B0%D0%BF%D0%BE%D0%BB%D0%BD%D0%B8%D1%82%D0%B5%D0%BB%D0%B8/fresh-step/74357/"

shops_list = ["Petshop", "Zoo812", "Kormkom", "Happypet"]

shops_map = {
		shops_list[0]:	PETSHOP_FRESHSTEP_URL,
		shops_list[1]:	ZOO812_FRESHSTEP_URL,
		shops_list[2]:	KORMKOM_FRESHSTEP_URL,
		shops_list[3]:	HAPPYPET_FRESHSTEP_URL
	}

def get_html(url):
	response = urllib2.urlopen(url)
	return response.read()


def parse_html(shop_key):

	html = get_html(shops_map[shop_key])
	soup = BeautifulSoup(html, "lxml")

	prices = {}
	if shops_list[0] == shop_key:
		type_inst = soup.find_all('div', class_='type-inst')
		offer_price = soup.find_all('span', class_='j_offer_price offer_price')

		for id in range(0, len(type_inst)):
			price = offer_price[id].text.strip().split()
			kg = float(type_inst[id].text.split()[0].replace(',', '.'))
			if len(price) > 2:
				prices[kg] = int(price[0] + price[1])
			else:
				prices[kg] = int(price[0])

	elif shops_list[1] == shop_key:
		kgs = soup.find_all('div', class_='large-2 columns name')
		costs = soup.find_all('div', class_='large-2 columns price')

		for id in range(0, len(kgs)):
			price = costs[id].text.strip().split()
			kg = float(kgs[id].text.split()[0].replace(',', '.'))
			if len(price) > 2:
				prices[kg] = int(price[0] + price[1])
			else:
				prices[kg] = int(price[0])

	elif shops_list[2] == shop_key:
		product_info_cost = soup.find('ul', id='productInfoCost')

		for price in product_info_cost.find_all('li'):
			kg = float(price.span.text.split()[0].replace(',', '.'))
			prices[kg] = int(price.find('p', class_='p2').text.split()[0])
	elif shops_list[3] == shop_key:
		kgs = soup.find_all('div', class_='sku-weight-card sku-weight-card-nocolor')
		costs = soup.find_all('div', class_='sku-price-card')

		for id in range(0, len(kgs)):
			price = costs[id].text.strip().split()
			kg = float(kgs[id].text.split()[0].replace(',', '.'))
			if len(price) > 7:
				prices[kg] = int(price[0] + price[1])
			else:
				prices[kg] = int(price[0])
	else:
		return None

	return prices


def getRowId(floatWeight):

	TRESHOLD = 0.5
	if abs(3.2 - floatWeight) < TRESHOLD:
		return 1
	elif abs(6.35 - floatWeight) < TRESHOLD:
		return 2
	elif abs(9.52 - floatWeight) < TRESHOLD:
		return 3
	elif abs(15.9 - floatWeight) < TRESHOLD:
		return 4
	else:
		return None


def getColId(strShop):

	if shops_list[0] == strShop:
		return 1
	elif shops_list[1] == strShop:
		return 2
	elif shops_list[2] == strShop:
		return 3
	elif shops_list[3] == strShop:
		return 4
	else:
		return None


def save_prices(price_list, path):

	wb = xlsxwriter.Workbook(path)
	ws = wb.add_worksheet("FreshStep")
	ws.set_tab_color('#8DB4E3')

	headerFmt = wb.add_format()
	headerFmt.set_bg_color('#538ED5')
	headerFmt.set_border(2)
	headerFmt.set_border_color("#FFFFFF")
	headerFmt.set_align('center')
	headerFmt.set_align('vcenter')
	headerFmt.set_font_color("#FFFFFF")
	headerFmt.set_bold()

	cornerFmt = wb.add_format()
	cornerFmt.set_bg_color('#FFFFFF')
	cornerFmt.set_border(2)
	cornerFmt.set_border_color("#FFFFFF")
	cornerFmt.set_align('center')
	cornerFmt.set_align('vcenter')
	cornerFmt.set_font_color("#00007F")
	cornerFmt.set_bold()

	weightFmt = wb.add_format()
	weightFmt.set_bg_color('#376091')
	weightFmt.set_border(2)
	weightFmt.set_border_color("#FFFFFF")
	weightFmt.set_align('center')
	weightFmt.set_align('vcenter')
	weightFmt.set_font_color("#FFFFFF")
	weightFmt.set_bold()

	generalFmt = wb.add_format()
	generalFmt.set_bg_color('#DBE5F1')
	generalFmt.set_align('center')
	generalFmt.set_align('vcenter')
	generalFmt.set_right(2)
	generalFmt.set_right_color("#FFFFFF")

	bestPriceFmt = wb.add_format()
	bestPriceFmt.set_bg_color('#55FF7F')
	bestPriceFmt.set_align('center')
	bestPriceFmt.set_align('vcenter')
	bestPriceFmt.set_right(2)
	bestPriceFmt.set_right_color("#FFFFFF")

	shopRowIdxConst = weightColIdxConst = 0

	ws.set_column(weightColIdxConst, weightColIdxConst, 20)
	ws.write(shopRowIdxConst, weightColIdxConst, "FreshStep", cornerFmt)

	priceMtrx = {}
	for kg in sorted(price_list[0][shops_list[0]].keys()):
		ws.write(getRowId(kg), weightColIdxConst, str(kg) + " kg", weightFmt)
		for shop in shops_list:
			ws.write(getRowId(kg), getColId(shop), "---", generalFmt)
			priceMtrx[(getRowId(kg), getColId(shop))] = 1000000

	for shop in price_list:
		for shop_key, price_value in shop.items():
			ws.set_column(getColId(shop_key), getColId(shop_key), 20)
			ws.write(shopRowIdxConst, getColId(shop_key), shop_key, headerFmt)
			for kg, price in price_value.items():
				ws.write(getRowId(kg), getColId(shop_key), str(price) + " rub", generalFmt)
				priceMtrx[(getRowId(kg), getColId(shop_key))] = price

	min_prices = {}
	for row in range(1, len(price_list[0][shops_list[0]].keys())+1):
		min = 1000000
		for col in range(1, len(shops_list)+1):
			if priceMtrx[(row, col)] < min:
				min = priceMtrx[(row, col)]
		for col in range(1, len(shops_list)+1):
			if priceMtrx[(row, col)] == min:
				ws.write(row, col, str(min) + " rub", bestPriceFmt)

	wb.close()


def main():

	price_list = []
	for key, value in shops_map.items():
		price_list.append({
			key: parse_html(key)
		})

	save_prices(price_list, os.path.join(os.getcwd(), "FreshStep.xlsx"))
	print("ALL DONE")


if __name__ == '__main__':
	main()
