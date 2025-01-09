# Navigate to root dir
cd $(dirname "$0")
path=$(pwd)

# Create venv
if [ -d "env" ]; then
    source $path/env/bin/activate
else
    python -m venv env
    source $path/env/bin/activate
    pip install pyTelegramBotAPI
fi

# Call python scripts
python GetInfo.py
