#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  	appCam.py
#  	based on tutorial ==> https://blog.miguelgrinberg.com/post/video-streaming-with-flask
# 	PiCam Local Web Server with Flask
# MJRoBot.org 19Jan18

from flask import Flask, render_template, Response ,request

# Raspberry Pi camera module (requires picamera package)
from camera_pi import Camera

 
app = Flask(__name__)
 
@app.route('/')
def index():
	return render_template('b.html')


@app.route('/<order>')
def control(order):

	return render_template('index.html')
	
	

def gen(camera):
    """Video streaming generator function."""
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    
	return Response(gen(Camera()),mimetype='multipart/x-mixed-replace; boundary=frame')

		
@app.route('/', methods=["GET","POST"])
def post():
	speed = request.form.get('num')
	
	return render_template('index.html')
 


if __name__ == '__main__':
	app.run(debug=True, host='0.0.0.0',port=5000)