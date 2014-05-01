require 'pathname'

ROOT = Pathname.new(__FILE__).join('..').expand_path

def search_files(*path_fragments)
  Pathname.glob( ROOT.join(*path_fragments) )
end

def run(command)
  puts(command)
  system(command)
end

desc 'Clean the build directory'
task :clean do
  ROOT.join('bin').rmtree
  ROOT.join('build').rmtree
end

task :setup_build_directory do
  ROOT.join('bin').mkpath
  ROOT.join('build').mkpath
  ROOT.join('build', 'src').mkpath
  ROOT.join('build', 'lib').mkpath
  ROOT.join('build', 'tools').mkpath
end

namespace :ruby do
  
  desc 'Transcompile Ruby files into C files'
  task :transcompile => :setup_build_directory do
    search_files('src', '**', '*.rb').each do |input_path|
      basename = input_path.basename('.rb')
      output_path = ROOT.join('build', 'src', "#{basename}.rb.c")
      
      run("mrbc -B #{basename} -o #{output_path} #{input_path}")
    end
  end
  
  desc 'Compile C files transcompiled from Ruby'
  task :compile => :transcompile do
    search_files('build', 'src', '**', '*.rb.c').each do |input_path|
      basename = input_path.basename('.rb.c')
      output_path = ROOT.join('build', 'src', "#{basename}.rb.o")
      
      run("gcc -o #{output_path} -c #{input_path}")
    end
  end
  
end

desc 'Transcompile, compile, and archive Ruby files'
task :ruby => 'ruby:compile'

namespace :c do
  
  desc 'Compile C files'
  task :compile => :setup_build_directory do
    search_files('src', '**', '*.c').each do |input_path|
      basename = input_path.basename('.c')
      output_path = ROOT.join('build', 'src', "#{basename}.o")
      
      run("gcc -o #{output_path} -c #{input_path}")
    end
  end
  
end

desc 'Compile and archive C files'
task :c => 'c:compile'

desc 'Archive all compiled files'
task :archive => [:ruby, :c] do
  compiled_files = search_files('build', 'src', '**', '*.o')
  output_path = ROOT.join('build', 'lib', 'libsimple.a')
  
  run("ar cr #{output_path} #{ compiled_files.join(' ') }")
end

namespace :tools do
  
  desc 'Compile tools'
  task :compile => :archive do
    tool_files = search_files('tools', '*.c')
    include_path = ROOT.join('include')
    
    tool_files.each do |input_path|
      basename = input_path.basename('.c')
      output_path = ROOT.join('build', 'tools', "#{basename}.o")
      
      run("gcc -I #{include_path} -o #{output_path} -c #{input_path}")
    end
  end
  
  desc 'Create tool binaries by linking against archive'
  task :link => :compile do
    tool_files = search_files('build', 'tools', '*.o')
    library_path = ROOT.join('build', 'lib', 'libsimple.a')
    
    tool_files.each do |input_path|
      basename = input_path.basename('.o')
      output_path = ROOT.join('bin', basename)
      
      run("gcc #{input_path} #{library_path} -l mruby -o #{output_path}")
    end
  end
  
end

desc 'Compile all tool binaries'
task :tools => 'tools:link'

task :default => :tools
