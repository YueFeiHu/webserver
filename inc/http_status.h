    //HTTP请求方法
    //HTTP请求方法,但本代码中仅仅支持GET
    enum METHOD{
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATCH
    };


    //主状态机可能的状态
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTION = 0,//正在分析当前请求行
        CHECK_STATE_HEADER,//正在分析头部字段
        CHECK_STATE_CONTENT
    };

    //从状态机可能的状态
    enum LINE_STATUS
    {
        LINE_OK = 0,//读取到一个完整的行
        LINE_BAD,//行出错
        LINE_OPEN//行数据尚且不完整
    };

    //服务器处理http请求的结果
    enum HTTP_CODE
    {
        NO_REQUEST,//请求不完整需要继续读取
        GET_REQUEST,//得到了一个完整的请求
        BAD_REQUEST,//请求有语法错误
        NO_RESOURCE,//没有资源
        FORBIDDEN_REQUEST,//没有足够的权限
        FILE_REQUEST,//文件已被请求
        INTERNAL_ERROR,//服务器内部错误
        CLOSED_CONNECTION//客户端连接已关闭
    };