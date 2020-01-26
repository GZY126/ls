#!/usr/bin/env  python2
# -*- coding: utf-8 -*-  

from flask import Flask


app = Flask(__name__)


@app.route('/API.php', methods=['POST'])
def API():   
    
    Ret = '06 02 00 00 00 a4 00 00 52 53 41 31 00 08 00 00 01 00 01 00 13 50 da 82 43 e4 bc b3 55 9d 84 f3 4e bd 9d ae 9c 4f ea 73 38 d3 7d 67 b3 b7 2b c3 d4 b2 48 aa 00 a3 f1 73 ab fa e0 3f 3a 34 7a 69 50 ab 78 50 ae f7 d0 40 1c 11 ef 3b 87 fa 94 27 98 d7 07 2e 01 ae a5 60 1a 33 90 00 42 f6 1f 42 a0 b9 73 d9 36 93 9f 8c 1b 43 75 50 92 2b e2 76 80 f6 fb 06 aa 33 97 a2 f4 b3 f7 44 a4 0a ad c3 e1 d1 92 23 d7 c8 f5 16 cb f7 ec 6b 29 45 9d 27 61 8b 02 2b 4a 14 2a 37 62 a9 25 59 90 55 f8 8c 47 94 b1 5a c1 16 4a 43 82 32 5b ba 39 1d 77 46 bd 6a f4 5b 01 2d 87 0a 6e df 1e 8a bc 87 e8 ae d4 ed c8 d2 20 85 d1 f9 7d 41 7e 9d 1c f3 01 4b 12 39 81 ae 76 08 ba f5 32 1c 37 d0 6d d8 63 b7 ac 58 14 84 df cf 0c b9 a0 96 e2 cb 94 a6 00 4c e6 6f 38 b2 e2 38 b9 fe 70 80 a7 ab 69 2b ee 19 30 f6 34 c1 e1 8e 71 f5 59 71 c9 a4 63 6a 67 54 2b 78 e8 a0'
    Ret = Ret.replace(' ', '')
    
    return '[1:' + Ret + ']'


@app.route('/mask.php', methods=['GET','POST'])
def MASK():   
    return '[0:jpg]'


@app.route('/Thanatos.php', methods=['GET','POST'])
def Thanatos():   
    return '1'


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, threaded=True)
