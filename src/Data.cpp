#include "Data.h"

Data::Data()
{
    imageLoader = shared_ptr<ofxThreadedImageLoader>(new ofxThreadedImageLoader());

}

void Data::load()
{
    ofDirectory dir;
    string folder;

    folder = "categories/256x256";
    dir.allowExt("png");
    dir.sort();
    dir.listDir(folder);

    categories.resize(dir.size());


    for(unsigned int i =0;i < dir.size();i++){
        string name = folder + "/" + dir.getName(i);
        imageLoader->loadFromDisk(categories[i],name);
    }
}
