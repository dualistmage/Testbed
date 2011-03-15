#encoding:utf-8
require 'rubygems'
require 'rest_client'
require 'json' 

# set function url
request_url = 'http://localhost/search_by_keywords'

# set function parameters
search_request = {
    "collection" => "SohuNews",
    "search" => {
        "keywords" => "中国",
        "in" => [ 
            { "property" => "Title" }, 
            { "property" => "Content" } 
        ],
        "ranking_model" => "plm",
        "log_keywords" => "true",
        "analyzer" : {
            "apply_la" 
    },
    "select" => ["Title", "Content"],
    "limit" => 10,
    "offset" => 0
}

# send http request to function url by rest client
# please insure to set "content_type" and "accept" 
#   "content_type" means the http body content type,  the http body is "search_request.to_json" 
#   "accept" represents for the result format, you can also set it to "xml" or "js" to get related format result

res = RestClient.post request_url, search_request.to_json, :content_type => :json, :accept => :json

puts "Result ---------------------------"
puts res

