#!/usr/bin/env python
from bottle import get,put,post,run,route,request
import os
import json
import sys

@route('/v1/rack/psu',method = 'POST')
def postpsu():
	print "--------------"
	print  request.body.getvalue()
	print "------------------"
#	json_data = json.load(request.body)
#	jdata = json.loads(json_data)
#	print "data is {}!!".format(json_data)
#	return "data is {}!!".format(json_data)
	return "psu data is {}!!".format(request.body.getvalue())

@route('/v1/rack/fan',method = 'POST')
def postfan():
	print "--------------"
	print  request.body.getvalue()
	print "------------------"
	return "fan data is {}!!".format(request.body.getvalue())

@route('/v1/rack/mbp',method = 'POST')
def postfan():
	print "--------------"
	print  request.body.getvalue()
	print "------------------"
	return "mbp data is {}!!".format(request.body.getvalue())

@route('/v1/rack/drawer',method = 'POST')
def postfan():
	print "--------------"
	print  request.body.getvalue()
	print "------------------"
	return "drawer data is {}!!".format(request.body.getvalue())

@route('/v1/rack/rack',method = 'POST')
def postfan():
	print "--------------"
	print  request.body.getvalue()
	print "------------------"
	return "rack data is {}!!".format(request.body.getvalue())


@route('/v1/rack',method = 'get')
def getrack():
	return "OK"

##listen the port forever,the host address id "0.0.0.0" means that it can accept any other host request!!!####
run(host='0.0.0.0',port=sys.argv[1])
