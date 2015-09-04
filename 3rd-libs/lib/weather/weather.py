#! /usr/bin/python2.7
#coding:utf-8
import sys  
import urllib2
import re
import requests
import json


'''
import sys
reload(sys)
sys.setdefaultencoding('utf8')
'''
def get_weather_data(city_name):
    url1 = 'http://wthrcdn.etouch.cn/weather_mini?city='+city_name  #只需城市名
    #url2 = 'http://wthrcdn.etouch.cn/weather_mini?citykey=101010100' #使用城市代码来查询
    weather_data = requests.get(url1).text #.content
    weather_dict = json.loads(weather_data)
    if weather_dict.get('desc') == 'OK':
        data = weather_dict.get('data')
        forecast = data.get('forecast')
        print (u'城市：'+data.get('city')).encode('UTF-8')
        print (u'日期：'+forecast[0].get('date')).encode('UTF-8')
        print u'温度：'+data.get('wendu')+u'℃'
        print u'天气：'+forecast[0].get('type')
        print u'风向：'+forecast[0].get('fengxiang')
        print u'风级：'+forecast[0].get('fengli')
#        print u'感冒：'+data.get('ganmao')
        print u'最高温度：'+forecast[0].get('high')[3:]
        print u'最低温度：'+forecast[0].get('low')[3:]
#        print u'是否查看未来四天的天气预报?[y/n]:',
        #yn = raw_input()
        yn = 'n' 
        if yn == 'y' or yn == 'Y':
            print '==========================='
            print u'     未来四天的天气预报     '
            print '==========================='
            for i in range(1,5):
                print u'日期：'+forecast[i].get('date')
                print u'最高温度：'+forecast[i].get('high')[3:]
                print u'最低温度：'+forecast[i].get('low')[3:]
                print u'天气：'+forecast[i].get('type')
                print u'风向：'+forecast[i].get('fengxiang')
                print u'风级：'+forecast[i].get('fengli')
                print '--------------------------'
    else:
        print u'查询失败,注意:错别字和非中国大陆地区会无法查询...'


def get_ip_physical_location(ip):
    #print(u'正在查询该ip的物理地址...')
    try:
        url1 = 'http://ip.taobao.com/service/getIpInfo.php?ip=' + ip
        url2 = 'http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=' + ip
        ip_data1 = requests.get(url1).text
        ip_dict1 = json.loads(ip_data1)
        if ip_dict1.get('code'):
            print(u'查询失败,正在切换新的查询链接...')
            ip_data2 = requests.get(url2).text
            ip_dict2 = json.loads(ip_data2)
            if ip_dict2.get('ret') == 1:
                print u'查询成功...'
                country = ip_dict2.get('country')
                province = ip_dict2.get('province')
                city = ip_dict2.get('city')
        else:
            data1 = ip_dict1.get('data')
            country = data1.get('country')
            province = data1.get('region')[0:-1] #因为会有个"省"字，另一个链接的城市又没有"省"字...
            city = data1.get('city')[0:-1] #因为会有个"市"字，另一个链接的城市又没有"市"字...
#        print(u'物理位置： ' + country + province + u"省" + city + u"市")
        return city
    except Exception,e:
        print u'查询物理位置失败...'
        print u'原因：\n%s' % e

def get_my_ip():
#    print u'正在查询您的ip地址...'
    url1 = 'http://ip.chinaz.com/'
    url2 = 'http://ip.dnsexit.com/'
    url3 = 'http://www.whereismyip.com/'
    my_ip = ''
    try:
        opener = urllib2.urlopen(url1,timeout=8)
        if url1 == opener.geturl():
            html = opener.read()
            my_ip = re.search('\d+\.\d+\.\d+\.\d+',html).group(0)
    except:
        try:
            opener = urllib2.urlopen(url2,timeout=10)
            if url2 == opener.geturl():
                html = opener.read()
                my_ip = re.search('\d+\.\d+\.\d+\.\d+',html).group(0)
        except:
            try:
                opener = urllib2.urlopen(url2,timeout=15)
                if url3 == opener.geturl():
                    html = opener.read()
                    my_ip = re.search('\d+\.\d+\.\d+\.\d+',html).group(0)
            except:
                print(u'联网查询外网ip失败...')
                my_ip = 'None'
    if my_ip != 'None':
        #print(u'您的外(公)网ip是:' + my_ip)
        return my_ip

if __name__ == '__main__':
    reload(sys)  
    sys.setdefaultencoding('utf8')
#    print u'正在联网查询您本地的天气...'
    get_weather_data(get_ip_physical_location(get_my_ip()))
#    print u'请输入需要查询天气的城市(如:广州、北京、上海):',
#    city_name = raw_input().decode('GBK') ###在命令行下需要转换成gbk格式！！！！！！！！！！！！！
    #city_name = city_name.decode('GBK') ###在命令行下需要转换成gbk格式！！！！！！！！！！！！！
#    get_weather_data(city_name)


