<img src="helv.gif"  />



LightNet* is a minimize neural network inference framework fork from Darknet. By hacking darknet, surprisingly I got a simple network running on lightweight mobile devices.

Choosing darknet help me got a simple solution. And for now I just prove it by running YOLOv3 object detection model tiny version, which can run with high FPS(22ms/52mAP) . For now everything unrelated was dropped, including cuda computing, trainning period and some layers.

Try it by compiling C code and run python demo. The api will return object names, probability and bounding boxs' axis. It works if you got predicition images like this,

![pred_giraffe](results/pred_giraffe.png)

### Features

- Small,  the core' size limit to 200kb
- Fast, written in C with performance optimization
- Hackable, thanks to the tiny size and understand it is easy
- Easy install, no external deps and just build by makefile in few seconds

### Future work

- Add more models and tasks
- Maybe NNPACK make it faster
- Make it easily for hacking 
- Wait for deep learning suitable for engineer hacking

###  Acknowledgement

Darknet is a fantastic project, the author also created YOLOnet. If you want hack it and have some fun, here are some useful links:

- Darknet official repo: https://github.com/pjreddie/darknet
- Darknet official site: https://pjreddie.com/darknet/
- YOLOv3: [Real-Time Object Detection](https://pjreddie.com/media/files/papers/YOLOv3.pdf)