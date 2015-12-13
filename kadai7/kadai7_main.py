#!/usr/bin/env python
#
# Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import webapp2
import urllib
import string

class MainPage(webapp2.RequestHandler):
    def get(self):
        message = self.request.get('message')
        str = message[::-1]
        self.response.out.write(str)
        self.response.headers['Content-Type'] = 'text/plain'

class SecondPage(webapp2.RequestHandler):
    def get(self):
        message = self.request.get('message')
        
        url = "http://step15-krispop.appspot.com/peers"
        result = urllib.urlopen(url)
        l = result.readlines()
        for x in l:
            #self.response.out.write(x)
            url_name = x.strip() + '/convert?message=' + message.strip()
            #self.response.out.write(url_name)
            
            result2 = urllib.urlopen(url_name)
            hold = result2.readlines()
            
            for y in hold:
                hold2 = y
                self.response.out.write(hold2 + '</br>')

class ThirdPage(webapp2.RequestHandler):
    def get(self):
        word = self.request.get('pos')

app = webapp2.WSGIApplication([
        ('/convert', MainPage),
        ('/show', SecondPage),
        ('/getword', ThirdPage),
], debug = True)
