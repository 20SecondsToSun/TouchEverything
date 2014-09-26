
#include "AssetsManager.h"

using namespace boost;
using namespace ci;
using namespace ci::app;
//using namespace ci::qtime;
using namespace gl;
using namespace std;

AssetManager* AssetManager::m_pInstance = NULL;
bool AssetManager::useThreadloading = false;

AssetManager* AssetManager::getInstance(){
    if (!m_pInstance)
        m_pInstance = new AssetManager;
    return m_pInstance;
}

AssetManager::~AssetManager(void){
    // stop loader thread and wait for it to finish
    mThread->interrupt();
    mThread->join();

    // clear buffers
    mTextureAssets.clear();
    mUrls.clear();
    mLoadedSurfaces.clear();
}

void AssetManager::setup(){
    if(!mIsSetup){
        if(useThreadloading){
            mThread = boost::shared_ptr<boost::thread>(new boost::thread(&AssetManager::threadLoad, this));
        }
        mIsSetup = true;
    }
}

string AssetManager::getAssetPath(){
    // returns folder of the App for relative paths
    if(mAssetPath.empty()){
        mAssetPath = getAppPath().string();
        mAssetPath = mAssetPath.substr( 0, mAssetPath.find_last_of("/")+1 );
    }
    return mAssetPath;
}

string AssetManager::getResourcePath(){
    // returns folder of the App for relative paths
    if(mResourcePath.empty()){
        mResourcePath = getAppPath().string();
        mResourcePath += "/Contents/Resources/";
        //console()<< " mResourcePath: " << mResourcePath << endl; 
    }
    return mResourcePath;
}


Texture* AssetManager::getTexture( string url, bool loadInThread ){
	
    setup();
	//console()<<"getTexture ..  "<<std::endl;
    if(mTextureAssets[url]){
        return &mTextureAssets[url];
    }else{
        mTextureAssets[url] = Texture(1,1);
        if(loadInThread && useThreadloading){
            mTextureAssets[url] = Texture(1,1);
            mUrlsMutex.lock();
            mUrls.push_back(url);
            mUrlsMutex.unlock();
			//console()<<"loadInThread ..  "<<std::endl;
        }else{
            try{               
                 // try to load relative to app
				console()<<"TRY LOAD TEXTURE ..  "<< url <<std::endl;
				 mTextureAssets[url] = Texture( loadImage( loadAsset( url ) ) ); 
				 console()<<"OK ..  "<< url <<std::endl;
            }
			catch(...)
			{
                try { 
                 console()<<"ERROR LOAD TEXTURE ..  "<<std::endl;
					 // try to load from URL
                   mTextureAssets[url] = Texture( loadImage( loadUrl( Url( url ) ) ) ); 
                }
                catch(...) { 
                    try {                       
                        //try loading from resource folder
						// mTextureAssets[url] = Texture( loadImage( loadResource( url ) ) );
                    }
                    catch(...) {
                        console() << getElapsedSeconds() << ":" << "Failed to load:" << url << endl;
                    }
                }
            }
        }
        return &mTextureAssets[url];
    }
}

/*MovieGl* AssetManager::getMovieGL( string url){
    setup();
    MovieGl *m;
    try{
		  //try load relative to app
            m = new MovieGl( getAssetPath() + url );
       
    }catch(...){
        try{
           //try load from resource folder
			// m = new MovieGl( loadResource( url ) );
        }catch(...){
            console() << getElapsedSeconds() << ":" << "Failed to load MovieGL:" << url << endl;
        }
    }
    return m;
}*/
/*
vector<Texture *> AssetManager::getTextureListFromDir( string filePath, bool loadInThread){
    //currently only loads from 
    setup();
    vector<Texture *> textures;
    textures.clear();
    fs::path p( getAssetPath() + filePath );
    //console()<< "assetPath " << a << ", exists? :: " << fs::is_directory(a)<<":"<< fs::exists(a) << endl;
    if( !fs::is_directory(p) && !fs::exists(p)){
        p = fs::path(ci::app::App::getAssetPath().string() + "/" + filePath );
    }
    if( !fs::is_directory(p) && !fs::exists(p)){
        console() << "AssetManager::getTextureListFromDir ERROR: folder \"" << filePath << " does neither exist as relative path nor as Resource path!" <<endl; 
    }
    for ( fs::directory_iterator it( p ); it != fs::directory_iterator(); ++it ){
        if ( fs::is_regular_file( *it ) ){
            // -- Perhaps there is a better way to ignore hidden files
            string fileName =  it->path().filename().string();
            if( !( fileName.compare( ".DS_Store" ) == 0 ) ){
                //console() << "AssetManager::getTextureListFromDir loading: " << fileName << endl;
                textures.push_back( getTexture( filePath + fileName, loadInThread ) );
                //textureAssets[filePath + fileName] = Texture(1,1);
                //textures.push_back( &textureAssets[filePath + fileName] );
            }
        }
    }
    return textures;
}*/



void AssetManager::update(){
    mLoadedSurfacesMutex.lock();
    int count = 0;
    while (!mLoadedSurfaces.empty() && count < 3){
        map<string, Surface>::iterator mitr = mLoadedSurfaces.begin();
        mTextureAssets[(*mitr).first] = Texture( (*mitr).second );
        console()<< "created: " << (*mitr).first << endl;
        mLoadedSurfaces.erase( (*mitr).first );
        count++;
    }
    mLoadedSurfacesMutex.unlock();
}

void AssetManager::threadLoad(){
	bool			empty;
	bool			succeeded;
	Surface			surface;
	ImageSourceRef	image;
	string			url;
    
	// run until interrupted
	while(true) {
		// fetch first url from the queue, if any	
		mUrlsMutex.lock();
		empty = mUrls.empty();
		if(!empty) url = mUrls.front();
		mUrlsMutex.unlock();
        
		if(!empty) {
			// try to load image
			succeeded = false;
            
            // try to load from resource folder file
			/*if(!succeeded) try { 
				image = loadImage( loadResource( url ) ); 
				succeeded = true;
			} catch(...) {}*/
            
			// try to load from relative file
			if(!succeeded) try { 
				//console()<<"LOAD!!!!!!!!!!!!!!"<<std::endl;
				image =  loadImage( loadAsset( url ) ); 
				succeeded = true;
			} catch(...) {}
            
			// try to load from URL
			if(!succeeded) try { 
				image = ci::loadImage( ci::loadUrl( Url(url) ) ); 
				succeeded = true;
			} catch(...) {}
            
			if(!succeeded) {
				// both attempts to load the url failed
                
				// remove url from queue
				mUrlsMutex.lock();
				mUrls.pop_front();
				mUrlsMutex.unlock();
                console() << "AssetManager Loading Error: Image not found.:  " <<url<<endl;
				continue;
			}
            
			// create Surface from the image
			surface = Surface(image);
            
			// check if thread was interrupted
			try { boost::this_thread::interruption_point(); }
			catch(boost::thread_interrupted) { break; }
            
			// copy to main thread
            mLoadedSurfacesMutex.lock();
            mLoadedSurfaces[url] = surface;
            mLoadedSurfacesMutex.unlock();

			// remove url from queue
			mUrlsMutex.lock();
            mUrls.pop_front();
            mUrlsMutex.unlock();
		}
		// sleep a while
		boost::this_thread::sleep( boost::posix_time::milliseconds(10) );
        //boost::thread::yield();
	}
}