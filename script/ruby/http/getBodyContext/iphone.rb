#encoding:utf-8
require 'net/http'
require 'uri'
require 'iconv'

for i in 0..99999999 do

    url = URI.parse('http://phonestore.show.co.kr/handler/Model-ProductList?modelId=' + '%08d' % i)
    req = Net::HTTP::Get.new(url.path)
    res = nil
    utfbody = nil
    begin
        Timeout::timeout(5) do
            res = Net::HTTP.start(url.host, url.port) do |http|
                http.request(req)
            end
            utfbody = Iconv.iconv('utf-8', 'euc-kr', res.body).join
        end

    rescue Timeout::Error
        utfbody = 'iphone'
    end


    #puts utfbody
    puts url.to_s
    if utfbody.match('아이폰|iphone') != nil
    puts "아이폰 아이폰 아이폰 iphone Iphone iPhone" 
    end


end
