#include "rtsp_handler.h"

GMainLoop *loop;
GstRTSPServer *server;
GstRTSPMediaMapping *mapping;
GstRTSPMediaFactory *factory;

rtsp_handler::rtsp_handler(QObject *parent)
{
    rtspServer = new rtsp_server(1, this);
    setupServer();
}

/*define to enable user/admin pw*/
#undef WITH_AUTH

/*this timeout is periodically run to clean up expired sessions*/
static gboolean timeout(GstRTSPServer * server, gboolean ignored)
{
   // Q_UNUSED(ignored);

    GstRTSPSessionPool *pool;

    pool = gst_rtsp_server_get_session_pool(server);
    gst_rtsp_session_pool_cleanup(pool);
    g_object_unref(pool);

    return true;
}

void rtsp_handler::setupServer()
{

#ifdef WITH_AUTH
    GstRTSPAuth *auth;
    gchar *basic;
#endif


    loop = g_main_loop_new(NULL, false);

    //create server instance
    server = gst_rtsp_server_new();

    /*get the mapping for this server, every server has a default mapper object
      *that can be used to map uri mount points to media factories*/
    mapping = gst_rtsp_server_get_media_mapping(server);

#ifdef WITH_AUTH
    /* make a new authentication manager. it can be added to control access to all
      * the factories on the server or on individual factroies*/
    auth = gst_rtsp_auth_new();
    basic = gst_rtsp_auth_make_basic("user", "admin");
    gst_rtsp_auth_set_basic(auth, basic);
    g_free(basic);
    /*congifure server*/
    gst_rtsp_server_set_auth(server, auth);
#endif

    /*make a media factory for a test stream. The default media factory can use
     * gst-launch syntax to create pipelines
     * any launch line works as long as it contains elements named pay%d
     * Each element w ith pay%d will be a stream*/
    factory = gst_rtsp_media_factory_new();

}

void rtsp_handler::start()
{    
    rtspServer->start(QThread::TimeCriticalPriority);
}

rtsp_server::rtsp_server(int ID, QObject *parent) :
    QThread(parent)
{
    QObject::connect(this, SIGNAL(serve()), this, SLOT(beginServe()));
    qDebug() << "RtspThread ID: " << ID  << " this thread: "<< this<< " Parent: " << parent;
}

void rtsp_server::run()
{
    emit beginServe();
    exec();
}

void rtsp_server::beginServe()
{
    /*set launch & run main loop && exec()*/

#ifdef IMX6
    gst_rtsp_media_factory_set_launch(factory, "("
                                      "mfw_v4lsrc capture-mode=0 ! video/x-raw-yuv,width=640,height=480,framerate=15/1 ! "
                                      "ffmpegcolorspace ! vpuenc codec=6  ! rtph264pay name=pay0 pt=96 " ")");
#else

    gst_rtsp_media_factory_set_launch(factory, "("
                                      "v4l2src ! video/x-raw-yuv,width=640,height=480,framerate=15/1 ! "
                                      "ffmpegcolorspace ! x264enc ! rtph264pay name=pay0 pt=96 " ")");
#endif


    gst_rtsp_media_factory_set_shared(factory, true);

    //attach the test factory to the /test url
    gst_rtsp_media_mapping_add_factory(mapping, "/test", factory);
    g_print("%s",gst_rtsp_server_get_address(server));

    g_object_unref(mapping);

    //attach the server to the default maincontext
    if(gst_rtsp_server_attach(server, NULL) == 0){
        g_print("failed to attach the server") ;
        QApplication::exit(-1);
    }

    //add timeout for cleanup
    g_timeout_add_seconds(2, (GSourceFunc)timeout, server);

    //start serving
    exec();
    g_main_loop_run(loop);
}

void rtsp_handler::updateVidSrcCnfg()
{
    QString framerate = QString("15");
    QString width = QString("640");
    QString height = QString("480");

    emit sendVidSrcCnfg(width , height, framerate);
}


