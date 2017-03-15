#include "Data.h"

Data::Data()
{
    imageLoader = shared_ptr<ofxThreadedImageLoader>(new ofxThreadedImageLoader());

}

void Data::load()
{
    ofDirectory dir;
    string folder;

    folder = "categories";
    dir.allowExt("png");
    dir.sort();
    dir.listDir(folder);

    categories.resize(dir.size());


    for(unsigned int i = 0;i < dir.size();i++){
        string name = folder + "/" + dir.getName(i);
        imageLoader->loadFromDisk(categories[i],name);
    }

   // folder = "animations";
    folder = "videos";
    dir.allowExt("mov");
    dir.listDir(folder);
    dir.sort();
    for(unsigned int i = 0;i < dir.size();i++){
        //if(dir[i].isDirectory()) {
            //shared_ptr<ofxVideoBufferPlayer> player = shared_ptr<ofxVideoBufferPlayer>(new ofxVideoBufferPlayer());
            shared_ptr<ofVideoPlayer> player = shared_ptr<ofVideoPlayer>(new ofVideoPlayer());
            player->setLoopState(OF_LOOP_NORMAL);
            player->setPixelFormat(OF_PIXELS_RGBA);
            animations.push_back(player);
            //animations.back()->load(folder +"/" + dir.getName(i));
            animations.back()->load(folder +"/" + dir.getName(i));
        //}
    }

    //animations[0].setLoopInterval(85,120); // habitat fps 15
    //animations[0]->setLoopInterval(10,27); // travel fps 5
}
