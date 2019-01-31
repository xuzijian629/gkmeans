def assert_run s
    `#{s}`
    stat = `echo $?`.chomp
    if stat != "0"
        raise "Failed to execute following command : #{s}"
    end
end

puts "Erase data and result before running? (preffered) y/n"
res = gets.chomp
if res == 'y'
    `rm ./data/*`
    `rm ./result/*`
    `rm ./tmp/*`
end

puts "Compiling files..."
assert_run("g++ --std=c++17 -O2 sb_gen.cpp -o tmp/sb_gen")
assert_run("g++ --std=c++17 -O2 gkmeans/gkmeanspp.cpp -o tmp/gkmeanspp")
assert_run("g++ --std=c++17 -O2 spectral/unnormalized.cpp -o tmp/unnormalized")
assert_run("g++ --std=c++17 -O2 spectral/normalized.cpp -o tmp/normalized")

puts "Generating datasets..."
assert_run("cat ./conf/1 | ./tmp/sb_gen")
assert_run("cat ./conf/2 | ./tmp/sb_gen")
assert_run("cat ./conf/3 | ./tmp/sb_gen")
# assert_run("cat ./conf/4 | ./tmp/sb_gen")

puts "Running algorithms..."
Dir.foreach('./data') do |data|
    if data["sbm"]
        for i in 1..20
            puts "#{i}-th round for #{data}"
            assert_run("echo data/#{data} | ./tmp/gkmeanspp > result/gkmeanspp_#{data}_#{i}")
        end

        for i in 1..3
            puts "#{i}-th round for #{data}"
            assert_run("echo data/#{data} | ./tmp/unnormalized > result/unnormalized_#{data}_#{i}")
            assert_run("echo data/#{data} | ./tmp/normalized > result/normalized_#{data}_#{i}")
        end
    end
end