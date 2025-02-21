import sys
import glob
import telebot
from git import Repo

def CalculateLinesOfCode(directory, file_types):
    lines = 0
    files = 0

    for file_name in glob.glob(directory + "/**/*", recursive=True):
        for file_type in file_types:
            if (file_name.endswith(file_type)):
                with open(file_name, 'r') as file:
                    lines += len(file.readlines())
                    files += 1
                break

    return lines, files


with open("../../Build/report.md") as reportFile:
    # Tests
    testsData = [line.rstrip() for line in reportFile.readlines()]

    totalTests = int(testsData[0].split(" ")[1])
    passedTests = int(testsData[1].split(" ")[1])
    failedTests = int(testsData[2].split(" ")[1])

    report = "  Passed tests: <b>" + str(passedTests) + "</b>\n\n"
    report = "  Failed tests: <b>" + str(failedTests) + "</b>\n" + report
    report = "Total tests: <b>" + str(totalTests) + "</b>\n" + report

    repo = Repo('./../..')
    commit = list(repo.iter_commits('master'))[0]

    report = "Message: " + commit.summary + "\n\n" + report
    if (failedTests != 0):
        report = commit.hexsha[0:11] + ": <b>failed</b>\n\n" + report
    else:
        report = commit.hexsha[0:11] + ": <b>success</b>\n\n" + report

    # Code lines
    file_types = [".h", ".c", ".hpp", ".cpp"]
    srcLines, srcFiles = CalculateLinesOfCode("../../Source", file_types)
    testsLines, testsFiles = CalculateLinesOfCode("../../Tests", file_types)
    examplesLines, examplesFiles = CalculateLinesOfCode("../../Examples", file_types)

    report += "Lines\n"
    report += "  Total: " + str(srcLines + testsLines + examplesLines) + "\n"
    report += "  Source: " + str(srcLines) + "\n"
    report += "  Tests: " + str(testsLines) + "\n"
    report += "  Examples: " + str(examplesLines) + "\n\n"

    report += "Files\n"
    report += "  Total: " + str(srcFiles + testsFiles + examplesFiles) + "\n"
    report += "  Source: " + str(srcFiles) + "\n"
    report += "  Tests: " + str(testsFiles) + "\n"
    report += "  Examples: " + str(examplesFiles)

    # Telegram
    botTimeWeb = telebot.TeleBot(sys.argv[1])
    if len(sys.argv) == 3:
        botTimeWeb.send_message(sys.argv[2], report, parse_mode='html', reply_markup=telebot.types.InlineKeyboardMarkup())
    elif len(sys.argv) == 4:
        botTimeWeb.send_message(sys.argv[2], report, parse_mode='html', reply_markup=telebot.types.InlineKeyboardMarkup(), message_thread_id=sys.argv[3])
