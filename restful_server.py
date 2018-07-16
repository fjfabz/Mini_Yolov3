import flask
from ctypes import *
import collections

app = flask.Flask(__name__)

class object(Structure):
    _fields_ = [("name", c_char_p),
                ("prob", c_float),
                ("x", c_float),
                ("y", c_float),
                ("w", c_float),
                ("h", c_float)]


class objects(Structure):
    _fields_ = [("objs", POINTER(object)),
                ("cnt", c_int)]

class Net:
    def __init__(self):
        lib = CDLL("libdarknet.so", RTLD_GLOBAL)
        load_net = lib.load_net
        load_net.argtypes = []
        load_net.restype = c_void_p

        self.detect = lib.detect
        self.detect.argtypes = [c_char_p,c_char_p]
        self.detect.restype = objects

        load_net()
        
    
@app.route("/detect", methods=["POST"])
def detect():
    if flask.request.method == "POST":
        if flask.request.files.get("image"):
            img = flask.request.files["image"]
            upload_name = 'results/'+img.filename
            out_name = 'results/pred_'+img.filename

            img.save(upload_name)
            data = {'predictions:':[]}
            objs = net.detect(bytes(upload_name,encoding='utf8'),bytes(out_name,encoding='utf8'))
            for i in range(objs.cnt):
                pred = collections.OrderedDict()
                pred['label'] = bytes.decode(objs.objs[i].name)
                pred['prob'] = round(objs.objs[i].prob ,2)
                pred['x'] = round(objs.objs[i].x * 100,2)
                pred['y'] = round(objs.objs[i].y * 100,2)
                pred['w'] = round(objs.objs[i].w * 100,2)
                pred['h'] = round(objs.objs[i].h * 100,2)
             
                data['predictions:'].append(pred)
             
            os.remove(upload_name)
            os.remove(out_name)
            return flask.jsonify(data)
  
if __name__ == '__main__':
    net = Net()
    app.run()