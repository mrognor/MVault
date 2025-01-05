import sys
import telebot

botTimeWeb = telebot.TeleBot(sys.argv[1])
botTimeWeb.send_message(sys.argv[2], "CI jobs finished!", parse_mode='html', reply_markup=telebot.types.InlineKeyboardMarkup())