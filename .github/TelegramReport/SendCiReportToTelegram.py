import sys
import telebot

with open("../../Build/Tests/report.md") as reportFile:
    report = reportFile.read()

    totalTests = 0
    passedTests = 0
    failedTests = 0

    words = report.split(" ")
    for i in range(len(words)):
        if words[i] == "Tests:":
            totalTests += int(words[i + 1][3:-5])
        if words[i] == "Passed:":
            passedTests += int(words[i + 1][3:-5])
        if words[i] == "Failed:":
            failedTests += int(words[i + 1][3:-5])

    
    report = "  Passed tests: <b>" + str(passedTests) + "</b>\n\n" + report
    report = "  Failed tests: <b>" + str(failedTests) + "</b>\n" + report
    report = "Total tests: <b>" + str(totalTests) + "</b>\n" + report

    if (failedTests != 0):
        report = "Status: <b>failed</b>\n\n" + report
    else:
        report = "Status: <b>success</b>\n\n" + report

    botTimeWeb = telebot.TeleBot(sys.argv[1])
    if len(sys.argv) == 3:
        botTimeWeb.send_message(sys.argv[2], report, parse_mode='html', reply_markup=telebot.types.InlineKeyboardMarkup())
    elif len(sys.argv) == 4:
        botTimeWeb.send_message(sys.argv[2], report, parse_mode='html', reply_markup=telebot.types.InlineKeyboardMarkup(), message_thread_id=sys.argv[3])
