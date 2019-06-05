#!/usr/bin/env python


from flask import Flask, render_template, Response

app = Flask(__name__)

 
 
 
@app.route('/')
def index():
    return render_template('a.html')


	


		


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
