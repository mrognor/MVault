# Navigate to root dir
cd $(dirname "$0")
path=$(pwd)

# Create venv
python -m venv env
source $path/env/bin/activate
pip install pyTelegramBotAPI

# Call python scripts
python SendCiReportToTelegram.py $1 $2