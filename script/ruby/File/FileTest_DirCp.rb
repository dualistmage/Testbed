#encoding:utf-8

require 'fileutils'

dir = File.dirname(File.expand_path(__FILE__))

#create dir

def createDir path
    'mkdir -p #{path}'
end

createDir 'testDir'
