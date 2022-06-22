/**
 *\copyright    XT Tech. Co., Ltd.
 *\file         main.c
 *\author       xt
 *\version      1.0.0
 *\date         2022-02-08
 *\brief        主模块实现,UTF-8(No BOM)
 */
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <winuser.h>
#include <commctrl.h>
#include "resource.h"
#include "config.h"
#include "xt_log.h"
#include "xt_md5.h"
#include "xt_base64.h"
#include "xt_pinyin.h"
#include "xt_timer.h"
#include "xt_thread_pool.h"
#include "xt_memory_pool.h"
#include "xt_notify.h"
#include "xt_http.h"
#include "xt_exe_ico.h"
#include "xl_sdk.h"
#include "torrent.h"

/// 程序标题
#define TITLE "DownloadSDKServerStart"

/// 主页面
#define INDEX_PAGE "<meta charset='utf-8'>\
<script>\n\
    function download(init){\n\
        filename = document.getElementById('file').value;\n\
        if (filename == '' && !init) {alert('请输入要下载的文件地址');return;}\n\
        url = '/download?file=' + filename;\n\
        file_table = document.getElementById('file-list');\n\
        if (file_table.style.display == '') {\n\
            list = '';\n\
            file_tbody = file_table.childNodes[0];\n\
            for (var i = 1; i < file_tbody.childNodes.length; i++){\n\
                list = list + (file_tbody.childNodes[i].childNodes[0].childNodes[0].checked ? '1' : '0');\n\
            }\n\
            if (/^0+$/.test(list)) {alert('请选取要下载的文件'); return;}\n\
            url = url + '&list=' + list ;\n\
            file_table.style.display = 'none';\n\
        }\n\
        req = new XMLHttpRequest();\n\
        req.open('GET', url);\n\
        req.send(null);\n\
        req.onload = function(){\n\
            console.log(url + ' status:' + req.status);\n\
            if (req.readyState != 4 || req.status != 200) {alert('http请求失败');return;}\n\
            rp = JSON.parse(req.responseText);\n\
            down_tbody = document.getElementById('down-list').childNodes[0];\n\
            while (down_tbody.childNodes.length > 1) { down_tbody.removeChild(down_tbody.childNodes[1]); }\n\
            for (var i in rp) {\n\
                item = rp[i];\n\
                tr = document.createElement('tr');\n\
                td = document.createElement('td');\n\
                td.innerText = item['id'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['torrent'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                file = decodeURIComponent(atob(item['file']));\n\
                td.id = 'file_' + i;\n\
                td.innerText = file;\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['size'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['speed'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['progress'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['time'];\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                if (/\\.torrent$/.test(file)) {\n\
                    btn = document.createElement('button');\n\
                    btn.innerText = 'open';\n\
                    btn.i = i;\n\
                    btn.id = 'btn_' + i;\n\
                    btn.style='display:block;';\n\
                    btn.onclick = show_in_torrent_files;\n\
                    td.appendChild(btn);\n\
                }\n\
                tr.appendChild(td);\n\
                down_tbody.appendChild(tr);\n\
            }\n\
        }\n\
    }\n\
    function show_in_torrent_files(){\n\
        this.style.display = 'none';\n\
        filename = document.getElementById('file_' + this.i).innerText;\n\
        url = '/torrent-list?torrent=' + filename\n\
        req = new XMLHttpRequest();\n\
        req.open('GET', url);\n\
        req.send(null);\n\
        req.onload = function(){\n\
            console.log(url + ' status:' + req.status);\n\
            if (req.readyState != 4 || req.status != 200) {alert('http请求失败');return;}\n\
            rp = JSON.parse(req.responseText);\n\
            document.getElementById('file').value = filename;\n\
            file_table = document.getElementById('file-list');\n\
            file_table.style.display = '';\n\
            file_tbody = file_table.childNodes[0];\n\
            while (file_tbody.childNodes.length > 1) { file_tbody.removeChild(file_tbody.childNodes[1]); }\n\
            for (var i in rp) {\n\
                item = rp[i];\n\
                tr = document.createElement('tr');\n\
                td = document.createElement('td');\n\
                ip = document.createElement('input');\n\
                ip.type = 'checkbox';\n\
                td.appendChild(ip);\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = decodeURIComponent(atob(item['file']));\n\
                tr.appendChild(td);\n\
                td = document.createElement('td');\n\
                td.innerText = item['size'];\n\
                tr.appendChild(td);\n\
                file_tbody.appendChild(tr);\n\
            }\n\
        }\n\
    }\n\
    function on_check(checkbox){\n\
        file_tbody = document.getElementById('file-list').childNodes[0];\n\
        for (var i = 1; i < file_tbody.childNodes.length; i++){\n\
            file_tbody.childNodes[i].childNodes[0].childNodes[0].checked = checkbox.checked;\n\
        }\n\
    }\n\
</script>\n\
<body onload='download(true)'>\
<input id='file'/>\
<button onclick='download(false)'>download</button>\
<table id='file-list' border='1' style='border-collapse:collapse;display:none'>\
<tr><th><input type='checkbox' name='check' onclick='on_check(this)' /></th><th>大小</th><th>文件</th></tr></table>\
<table id='down-list' border='1' style='border-collapse:collapse;'>\
<tr><th>任务</th><th>种子</th><th>文件</th><th>大小</th><th>速度</th><th>进度</th><th>用时</th><th>操作</th></tr>\
</table>\
</body>"
config                  g_cfg           = {0};  ///< 配置数据

xt_log                  g_log           = {0};  ///< 日志数据
xt_log                  g_test          = {0};  ///< 多日志测试

xt_http                 g_http          = {0};  ///< HTTP服务
xt_thread_pool          g_thread_pool   = {0};  ///< 线程池
xt_timer_set            g_timer_set     = {0};  ///< 定时器

bt_torrent              g_torrent       = {0};  ///< 种子文件信息
xl_task                 g_task[128]     = {0};  ///< 当前正在下载的任务信息
int                     g_task_count    = 0;    ///< 当前正在下载的任务数量

/**
 *\brief        得到格式化后的信息
 *\param[in]    data            数据
 *\param[out]   info            信息
 *\param[in]    info_size       缓冲区大小
 *\return                       无
 */
void format_data(unsigned __int64 data, char *info, int info_size)
{
    double g = (double)data / (1024.0 * 1024 * 1024);
    double m = (double)data / (1024.0 * 1024);
    double k = (double)data / (1024.0);

    double g1 = data / 1024.0 / 1024.0 / 1024.0;
    double m1 = data / 1024.0 / 1024.0;
    double k1 = data / 1024.0;

    DBG("g:%f m:%f k:%f", g, m, k);
    DBG("g1:%f m1:%f k1:%f", g1, m1, k1);

    if (g >= 0.9)
    {
        snprintf(info, info_size, "%.2fG", g);
    }
    else if (m >= 0.9)
    {
        snprintf(info, info_size, "%.2fM", m);
    }
    else if (k >= 0.9)
    {
        snprintf(info, info_size, "%.2fK", k);
    }
    else
    {
        snprintf(info, info_size, "%I64u", data);
    }
}

/**
 *\brief        下载文件
 *\param[in]    filename        文件地址
 *\param[in]    list            下载BT文件时选中的要下载的文件,如:"10100",1-选中,0-末选
 *\return       0               成功
 */
int xl_download(const char *filename, const char *list)
{
    int ret;
    int task_id;
    int task_type;
    char task_name[MAX_PATH];

    DBG("task count:%d", g_task_count);

    for (int i = 0; i < g_task_count; i++)
    {
        if (0 == strcmp(filename, g_task[i].filename))  // 已经下载
        {
            DBG("have %s", filename);
            return 0;
        }
    }

    if (0 == strcmp(filename + strlen(filename) - 8, ".torrent"))   // BT下载
    {
        if (NULL == list)
        {
            return -1;
        }

		task_type = TASK_BT;

        ret = xl_sdk_create_bt_task(filename, g_cfg.download_path, list, &task_id, task_name, sizeof(task_name));
    }
    else if (0 == strncmp(filename, "magnet:?", 8))                 // 磁力下载
    {
        task_type = TASK_MAGNET;

        ret = xl_sdk_create_magnet_task(filename, g_cfg.download_path, &task_id, task_name, sizeof(task_name));
    }
    else                                                            // 普通下载
    {
        task_type = TASK_URL;

        ret = xl_sdk_create_url_task(filename, g_cfg.download_path, &task_id, task_name, sizeof(task_name));
    }

    if (0 != ret)
    {
        ERR("create task %s error:%d", filename, ret);
        return -2;
    }

    ret = xl_sdk_start_download_file(task_id, task_type);

    if (0 != ret)
    {
        ERR("download start %s error:%d", filename, ret);
        return -3;
    }

    ret = xl_sdk_get_task_info(task_id, &g_task[g_task_count].size, &g_task[g_task_count].down, &g_task[g_task_count].time);

    if (0 != ret)
    {
        ERR("get task info error:%d", ret);
        return -4;
    }

    char size[16];
    format_data(g_task[g_task_count].size, size, sizeof(size));

    DBG("task:%d size:%s", task_id, size);

    strcpy_s(g_task[g_task_count].filename, sizeof(g_task[g_task_count].filename), filename);
    g_task[g_task_count].id   = task_id;
    g_task[g_task_count].type = task_type;
    g_task[g_task_count].size = 0;
    g_task[g_task_count].down = 0;
    g_task[g_task_count].time = 0;

    /* test */
    strcpy_s(g_task[g_task_count].filename, sizeof(g_task[g_task_count].filename), "C:\\Program Files\\7-Zip\\1\\DB2FE78374A1A18C1A5EFCC5E961901A1BCACFD2.torrent");
    g_task[g_task_count].type = TASK_MAGNET;
    g_task[g_task_count].time = 123;
    g_task[g_task_count].size = 1024*1024*1024 + 235*1024*1024;

    g_task_count++;

    return 0;
}

/**
 *\brief        http回调函数,下载接口
 *\param[in]    arg             URI的参数
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_download(const p_xt_http_arg arg, p_xt_http_content content)
{
    const char *file = NULL;
    const char *list = NULL;

    if (arg->count >= 1 && NULL != arg->name[0] && 0 == strcmp(arg->name[0], "file") && 0 != strcmp(arg->data[0], ""))
    {
        file = arg->data[0];
    }

    if (arg->count >= 2 && NULL != arg->name[1] && 0 == strcmp(arg->name[1], "list") && 0 != strcmp(arg->data[1], ""))
    {
        list = arg->data[1];
    }

    if (NULL != file && 0 != xl_download(file, list))
    {
        ERR("xl_download fail");
        return -1;
    }

    int   pos = 1;
    int   len;
    int   encode_len;
    int   base64_len;
    char  size[16];
    char  encode[MAX_PATH];
    char  base64[MAX_PATH];
    char *data = content->data;

    data[0] = '[';

    for (int i = 0; i < g_task_count; i++)
    {
        encode_len = sizeof(encode);
        base64_len = sizeof(base64);

        uri_encode(g_task[i].filename, strlen(g_task[i].filename), encode, &encode_len); // js的atob不能解码unicode
        base64_encode(encode, encode_len, base64, &base64_len); // 文件名中可能有json需要转码的字符
        format_data(g_task[i].size, size, sizeof(size));

        len = snprintf(data + pos, content->len - pos,
                       "{\"id\":%d,\"torrent\":\"%s\",\"file\":\"%s\",\"size\":\"%s\","
                       "\"speed\":\"%s\",\"progress\":\"%s\",\"time\":%d},",
                       g_task[i].id, "", base64, size, "1.23M", "12.3", g_task[i].time);
        pos += len;
    }

    if (g_task_count > 0)
    {
        data[pos - 1] = ']';
    }
    else
    {
        data[pos++] = ']';
    }

    content->type = HTTP_TYPE_HTML;
    content->len = pos;
    return 0;
}

/**
 *\brief        http回调函数,得到种子中文件信息
 *\param[in]    arg             URI的参数
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_torrent(const p_xt_http_arg arg, p_xt_http_content content)
{
    if (arg->count <= 0 || NULL == arg->name[0] || 0 != strcmp(arg->name[0], "torrent"))
    {
        return -1;
    }

    const char *file = arg->data[0];

    if (NULL == file || 0 == strcmp(file, ""))
    {
        DBG("file:null or \"\"");
        return -2;
    }

    if (0 != get_torrent_info(file, &g_torrent))
    {
        ERR("get torrent:%s info error", file);
        return -3;
    }

    int   pos = 1;
    int   len;
    int   encode_len;
    int   base64_len;
    char  size[16];
    char  encode[MAX_PATH];
    char  base64[MAX_PATH];
    char *data = content->data;

    data[0] = '[';

    for (int i = 0; i < g_torrent.count; i++)
    {
        encode_len = sizeof(encode);
        base64_len = sizeof(base64);

        uri_encode(g_torrent.file[i].name, strlen(g_torrent.file[i].name), encode, &encode_len); // js的atob不能解码unicode
        base64_encode(encode, encode_len, base64, &base64_len); // 文件名中可能有json需要转码的字符
        format_data(g_torrent.file[i].size, size, sizeof(size));

        DBG("i:%d file:%s uri_encode:%s base64(uri_encode):%s", i, g_torrent.file[i].name, encode, base64);

        len = snprintf(data + pos, content->len - pos, "{\"file\":\"%s\",\"size\":\"%s\"},", base64, size);
        pos += len;
    }

    if (g_torrent.count > 0)
    {
        data[pos - 1] = ']';
    }
    else
    {
        data[pos++] = ']';
    }

    content->type = HTTP_TYPE_HTML;
    content->len = pos;
    return 0;
}

/**
 *\brief        http回调函数,文件
 *\param[in]    uri             URI地址
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_other(const char *uri, p_xt_http_content content)
{
    char file[512];
    sprintf_s(file, sizeof(file), "%s%s", g_cfg.http_path, uri);

    FILE *fp;
    fopen_s(&fp, file, "rb");

    if (NULL == fp)
    {
        ERR("open %s fail", file);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    content->len = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    fread(content->data, 1, content->len, fp);
    fclose(fp);

    DBG("%s size:%d", file, content->len);

    char *ext = strrchr(file, '.');

    if (NULL == ext)
    {
        content->type = HTTP_TYPE_HTML;
        return 0;
    }

    if (0 == strcmp(ext, ".co"))
    {
        content->type = HTTP_TYPE_ICO;
    }
    else if (0 == strcmp(ext, ".png"))
    {
        content->type = HTTP_TYPE_PNG;
    }
    else if (0 == strcmp(ext, ".jpg"))
    {
        content->type = HTTP_TYPE_JPG;
    }
    else if (0 == strcmp(ext, ".jpeg"))
    {
        content->type = HTTP_TYPE_JPEG;
    }
    else
    {
        content->type = HTTP_TYPE_HTML;
    }

    return 0;
}

/**
 *\brief        HTTP回调函数,/favicon.ico
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_icon(p_xt_http_content content)
{
    content->type = HTTP_TYPE_ICO;
    exe_ico_get_data(IDI_GREEN, content->data, &(content->len));
    return 0;
}

/**
 *\brief        HTTP回调函数,主页
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_index(p_xt_http_content content)
{
    content->type = HTTP_TYPE_HTML;
    content->len = sizeof(INDEX_PAGE) - 1;
    strcpy_s(content->data, sizeof(INDEX_PAGE), INDEX_PAGE);
    return 0;
}

/**
 *\brief        HTTP回调函数
 *\param[in]    uri             URI地址
 *\param[in]    arg             URI的参数,参数使用的是conten.data指向的内存
 *\param[out]   content         返回内容
 *\return       0               成功
 */
int http_proc_callback(const char *uri, const p_xt_http_arg arg, p_xt_http_content content)
{
    DBG("uri:%s", uri);

    if (0 == strcmp(uri, "/"))
    {
        return http_proc_index(content);
    }
    else if (0 == strcmp(uri, "/download"))
    {
        return http_proc_download(arg, content);
    }
    else if (0 == strcmp(uri, "/torrent-list"))
    {
        return http_proc_torrent(arg, content);
    }
    else if (0 == strcmp(uri, "/favicon.ico"))
    {
        return http_proc_icon(content);
    }
    else
    {
        return http_proc_other(uri, content);
    }

    return 404;
}

/**
 *\brief    定时器任务回调
 *\param    [in]  param         自定义参数
 *\return                       无
 */
void timer_callback(void *param)
{
    DBG("param:%s", (char*)param);
}

/**
 *\brief        初始化
 *\return       0               成功
 */
int init()
{
    char m[MAX_PATH];

    int ret = config_init(TITLE".json", &g_cfg);

    if (ret != 0)
    {
        sprintf_s(m, sizeof(m), "init config fail %d", ret);
        MessageBoxA(NULL, m, TITLE, MB_OK);
        return -10;
    }

    ret = log_init(g_cfg.log_filename, g_cfg.log_level, g_cfg.log_cycle, g_cfg.log_backup, g_cfg.log_clean, 38, &g_log);

    if (ret != 0)
    {
        sprintf_s(m, sizeof(m), "init log fail %d", ret);
        MessageBoxA(NULL, m, TITLE, MB_OK);
        return -20;
    }

    DBG("g_log init ok");

    ret = log_init(TITLE".test", g_cfg.log_level, g_cfg.log_cycle, g_cfg.log_backup, g_cfg.log_clean, 38, &g_test);

    if (ret != 0)
    {
        ERR("init log test fail %d", ret);
        return -21;
    }

    DBG("g_log init ok");
    DBG("g_log init ok");
    DBG("g_log init ok");
    DBG("g_log init ok");

    D(&g_test, "g_test init ok");
    D(&g_test, "g_test init ok");
    D(&g_test, "g_test init ok");
    D(&g_test, "g_test init ok");
    D(&g_test, "g_test init ok");

    DBG("--------------------------------------------------------------------");

    xt_md5 md5;
    char   md5_out[128];
    char  *md5_in = "1234567890";

    ret = md5_get(md5_in, strlen(md5_in), &md5);

    if (ret != 0)
    {
        ERR("get md5 fail %d", ret);
        return -30;
    }

    DBG("str:%s md5.A:%x B:%x C:%x D:%x", md5_in, md5.A, md5.B, md5.C, md5.D);

    ret = md5_get_str(md5_in, strlen(md5_in), md5_out);

    if (ret != 0)
    {
        ERR("get md5 str fail %d", ret);
        return -31;
    }

    DBG("str:%s md5:%s", md5_in, md5_out);

    md5_in = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"\
             "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"\
             "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"\
             "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"\
             "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"\
             "1234567890";

    ret = md5_get_str(md5_in, strlen(md5_in), md5_out);

    if (ret != 0)
    {
        ERR("get md5 str fail %d", ret);
        return -32;
    }

    DBG("str:%s md5:%s", md5_in, md5_out);

    DBG("--------------------------------------------------------------------");

    int len;
    char base64[64];
    char output[64];
    char data[][10] = { "1", "12", "123", "1234" };

    for (int i = 0; i < 4; i++)
    {
        len = sizeof(base64);

        ret = base64_encode(data[i], strlen(data[i]), base64, &len);

        if (ret != 0)
        {
            ERR("get base64 str fail %d", ret);
            return -40;
        }

        DBG("data:%s base64:%s len:%d", data[i], base64, len);

        ret = base64_decode(base64, len, output, &len);

        if (ret != 0)
        {
            ERR("from base64 get data fail %d", ret);
            return -41;
        }

        DBG("base64:%s data:%s len:%d", base64, output, len);
    }

    DBG("--------------------------------------------------------------------");

    ret = pinyin_init_res("PINYIN", IDR_PINYIN);

    if (ret != 0)
    {
        ERR("init pinyin fail %d", ret);
        return -50;
    }

    DBG("--------------------------------------------------------------------");

    xt_memory_pool mem_pool;

    ret = memory_pool_init(&mem_pool, 1024, 100);

    if (ret != 0)
    {
        ERR("init memory pool fail %d", ret);
        return -60;
    }

    void *mem = NULL;

    for (int i = 0; i < 2000; i++)
    {
        ret = memory_pool_get(&mem_pool, &mem);

        if (ret != 0)
        {
            ERR("memory pool get fail %d", ret);
            return -61;
        }

        DBG("memory_pool_get ret:%d count:%d list-size:%d count:%d head:%d tail:%d", ret, mem_pool.count,
        mem_pool.free.size, mem_pool.free.count, mem_pool.free.head, mem_pool.free.tail);
    }

    ret = memory_pool_put(&mem_pool, mem);

    if (ret != 0)
    {
        ERR("memory pool put fail %d", ret);
        return -62;
    }

    DBG("memory_pool_put ret:%d memory-pool-count:%d list-size:%d count:%d head:%d tail:%d", ret, mem_pool.count,
        mem_pool.free.size, mem_pool.free.count, mem_pool.free.head, mem_pool.free.tail);

    ret = memory_pool_uninit(&mem_pool);

    if (ret != 0)
    {
        ERR("memory pool uninit fail %d", ret);
        return -63;
    }

    DBG("--------------------------------------------------------------------");

    ret = thread_pool_init(&g_thread_pool, 10);

    if (ret != 0)
    {
        ERR("thread pool init fail %d", ret);
        return -70;
    }

    DBG("--------------------------------------------------------------------");

    ret = timer_init(&g_timer_set);

    if (ret != 0)
    {
        ERR("timer init fail %d", ret);
        return -80;
    }

    ret = timer_add_cycle(&g_timer_set, "timer_0",  5, &g_thread_pool, timer_callback, "timer_0_param");

    if (ret != 0)
    {
        ERR("add cycle timer fail %d", ret);
        return -81;
    }

    ret = timer_add_cycle(&g_timer_set, "timer_1", 10, &g_thread_pool, timer_callback, "timer_1_param");

    if (ret != 0)
    {
        ERR("add cycle timer fail %d", ret);
        return -82;
    }

    ret = timer_add_cron(&g_timer_set, "timer_2", TIMER_CRON_MINUTE, 0, 0, 0, 0, 0, 0, 0, &g_thread_pool, timer_callback, "timer_2_param");

    if (ret != 0)
    {
        ERR("add cron timer fail %d", ret);
        return -83;
    }

    DBG("--------------------------------------------------------------------");

    ret = http_init(g_cfg.http_port, http_proc_callback, &g_http);

    if (ret != 0)
    {
        ERR("http init fail %d", ret);
        return -90;
    }

    DBG("--------------------------------------------------------------------");

    // 初始化SDK
    ret = xl_sdk_init();

    if (0 != ret)
    {
        ERR("init error:%d", ret);
        return -100;
    }

    DBG("--------------------------------------------------------------------");

    ret = get_torrent_info("D:\\5.downloads\\bt\\7097B42EEBC037482B69056276858599ED9605B5.torrent", &g_torrent);

    if (0 != ret)
    {
        ERR("init error:%d", ret);
        return -110;
    }

    return 0;
}

/**
 *\brief        窗体显示函数
 *\param[in]    wnd             窗体句柄
 *\param[in]    param           自定义参数
 *\return                       无
 */
void on_menu_show(HWND wnd, void *param)
{
    ShowWindow(wnd, IsWindowVisible(wnd) ? SW_HIDE : SW_SHOW);
}

/**
 *\brief        窗体关闭处理函数
 *\param[in]    wnd             窗体句柄
 *\param[in]    param           自定义参数
 *\return                       无
 */
void on_menu_exit(HWND wnd, void *param)
{
    if (IDNO == MessageBoxW(wnd, L"确定退出?", L"消息", MB_ICONQUESTION | MB_YESNO))
    {
        return;
    }

    DestroyWindow(wnd);
}

/**
 *\brief        窗体类程序主函数
 *\param[in]    hInstance       当前实例句柄
 *\param[in]    hPrevInstance   先前实例句柄
 *\param[in]    lpCmdLine       命令行参数
 *\param[in]    nCmdShow        显示状态(最小化,最大化,隐藏)
 *\return                       exe程序返回值
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (init() != 0)
    {
        return -1;
    }

    notify_menu_info menu[2] = { {0, L"显示(&S)", NULL, on_menu_show},{1, L"退出(&E)", NULL, on_menu_exit} };

    notify_init(hInstance, IDI_GREEN, SIZEOF(menu), menu);

    // 消息体
    MSG msg;

    // 消息循环,从消息队列中取得消息,只到WM_QUIT时退出
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // 将WM_KEYDOWN和WM_KEYUP转换为一条WM_CHAR消息
        DispatchMessage(&msg);  // 分派消息到窗口,内部调用窗体消息处理回调函数
    }

    return (int)msg.lParam;
}

/*
    int error;
    PCRE2_SIZE offset;
    PCRE2_SIZE *ovector;

    char *pattern = "{0-9}{5}";

    pcre2_code *pcre_data = pcre2_compile((PCRE2_SPTR)pattern, 0, 0, &error, &offset, NULL);

    if (pcre_data == NULL)
    {
        PCRE2_UCHAR info[256];
        //pcre2_get_error_message(error, info, sizeof(info));
        ERR("PCRE init fail pattern:%s offste:%d err:%s", pattern, offset, error, info);
    }

    DBG("pcre2_compile ok pattern:\"%s\"", pattern);

    char *subject = "abcdefghijkl";

    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(pcre_data, NULL);

    ret = pcre2_match(pcre_data, (PCRE2_SPTR)subject, strlen(subject), 0, 0, match_data, NULL); // <0发生错误，==0没有匹配上，>0返回匹配到的元素数量

    if (ret > 0)
    {
        ovector = pcre2_get_ovector_pointer(match_data);

        for (int i = 0; i < ret; i++)
        {
            //int substring_length = ovector[2 * i + 1] - ovector[2 * i];
            //char* substring_start = subject + ovector[2*i];
            //DBG("%d: %d %.*s", i, substring_length, substring_start);
            DBG("%d:%d %d", i, ovector[2 * i], ovector[2 * i + 1]);
        }
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(pcre_data);

    DBG("pcre2_match subject:\"%s\" ret:%d", subject, ret);
*/
