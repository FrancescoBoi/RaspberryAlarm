import RPi.GPIO as GPIO
from flask import Flask, render_template
import os

LED_PIN = 4

app = Flask(__name__)
@app.route("/run")
def accendi():
    os.system("sudo ./alarm.o")
    print("Sistema attivato")
    return("Acceso")
    

@app.route("/view_pict")
def sview_pict():
    return render_template("foto.html")


if __name__ == "__main__":
    app.run(host="192.168.1.122")

