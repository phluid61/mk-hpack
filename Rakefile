#!/usr/bin/env rake

task :default => [:compile]

task :compile => 'index.html'

task 'index.html' => %w[head README.md LICENSE tail] do
	html = File.read('head')
	html << %x(./markdown README.md)
	html << %(<pre id="license" class="no-highlight">) << File.read('LICENSE') << %(</pre>)
	html << %x(./markdown code_of_conduct.md)
	html << File.read('tail')
	File.open('index.html', 'w') {|f| f.write(html) }
end

require 'rake/clean'
CLEAN.include %w[index.html]

