#include "darknet.h"

network* net;
char** names; 

objects get_detections(detection *dets, int num, float thresh, char **names,int classes)
{
    int i,j;
    objects objs;
    objs.cnt = -1;
    objs.objs = calloc(num, sizeof(object));
    for(i = 0; i < num; ++i){
        char labelstr[4096] = {0};
        int class = -1;
        for(j = 0; j < classes; ++j){
            if (dets[i].prob[j] > thresh){
                if (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } else {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                
                (objs.cnt)++;
                object obj = {names[j],dets[i].prob[j]*100,dets[i].bbox.x,
                            dets[i].bbox.y,dets[i].bbox.w,dets[i].bbox.h};
              
                objs.objs[objs.cnt] = obj;
        }
      }
    }
    objs.cnt++;
    return objs;
} 

objects detect(char* input,char* output)
{
    
    float thresh = 0.5 ,hier_thresh = 0.5 ,nms=0.45;
  
    image **alphabet = load_alphabet();


    image im = load_image_color(input,0,0);
   
    image sized = letterbox_image(im, net->w, net->h);
    
    layer l = net->layers[net->n-1];

    float *X = sized.data;
        
    network_predict(net, X);
      
    int nboxes = 0;
    detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
  
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
    
    objects objs = get_detections(dets, nboxes, thresh, names, l.classes);
    
    draw_detections(im, dets, nboxes, thresh, names, alphabet, l.classes);
    free_detections(dets, nboxes);
        
    save_image(im, output);
  
    free_image(im);
    free_image(sized);

    return objs;
}

void load_net()
{

    char *datacfg = "data/coco.data";
    char *cfg = "data/yolov3-tiny.cfg";
    char *weights = "data/yolov3-tiny.weights";

    list *options = read_data_cfg(datacfg);
    char *name_list = option_find_str(options, "names", "data/names.list");
    names = get_labels(name_list);

    net = load_network(cfg, weights, 0);
    set_batch_network(net, 1);
    srand(2222222);
}
