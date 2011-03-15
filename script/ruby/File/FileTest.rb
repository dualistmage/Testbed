#encoding:utf-8

require 'fileutils'

dir = File.dirname(File.expand_path(__FILE__))
sourcePath = File.join(dir,'hello.src')
targetPath = File.join(dir,'hello.tar')

puts "Source : #{sourcePath}"
puts "Target : #{targetPath}"
FileUtils.cp( sourcePath, targetPath )
