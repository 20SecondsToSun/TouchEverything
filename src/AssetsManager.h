#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Thread.h"
#include <string>
#include <boost/thread/thread.hpp>

using namespace ci;
using namespace ci::app;
//using namespace ci::qtime;
using namespace gl;
using namespace std;



class AssetManager {
  public:
    static AssetManager* getInstance();

    void setup();
    void update();
    
    string getAssetPath();
    string getResourcePath();
    Texture* getTexture( string path, bool loadInThread = true );
//    MovieGl* getMovieGL( string path );
    vector<Texture *> getTextureListFromDir( string filePath, bool loadInThread = true );

    static bool useThreadloading;
  private:
    AssetManager(){mIsSetup=false;}; // Private so that it can  not be called
    ~AssetManager();
    AssetManager(AssetManager const&){}; // copy constructor is private
    AssetManager& operator=(AssetManager const&){return *m_pInstance;}; // assignment operator is private
    static AssetManager* m_pInstance;
    
    boost::shared_ptr<boost::thread> mThread;
    
    map<string, Texture> mTextureAssets;
    string mAssetPath;
    string mResourcePath;
    
	boost::mutex mLoadedSurfacesMutex;
    map<string, Surface> mLoadedSurfaces;
    
    boost::mutex mUrlsMutex;
    deque<string> mUrls;
    
    bool mIsSetup;
    
    void threadLoad();
};