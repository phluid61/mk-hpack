#!/usr/bin/env ruby
require 'commonmarker'
puts Commonmarker.to_html(
  File.read(ARGV[0]),
  options: { render: { unsafe: true } }
)

