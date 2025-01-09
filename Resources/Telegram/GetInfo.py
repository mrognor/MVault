import os
import telebot

if os.path.isfile("BotToken"):

    with open("BotToken") as tokenFile:
        isOpened = True
        token = tokenFile.read()
        botTimeWeb = telebot.TeleBot(token)

        @botTimeWeb.message_handler(commands=['start'])
        def startBot(message):
            markup = telebot.types.InlineKeyboardMarkup()

            if (message.chat.is_forum):
                botTimeWeb.send_message(message.chat.id, "Chat id: " + str(message.chat.id) + ". Topic id: " + str(message.reply_to_message.message_thread_id), 
                    parse_mode='html', reply_markup=markup, message_thread_id=message.reply_to_message.message_thread_id)
            else:
                botTimeWeb.send_message(message.chat.id, "Chat id: " + str(message.chat.id), parse_mode='html', reply_markup=markup)

        botTimeWeb.infinity_polling()
else:
    print("Put bot token into BotToken file in this directory")