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
assert_run("g++ --std=c++17 -O2 gkmeans/gkmeans.cpp -o tmp/gkmeans")
assert_run("g++ --std=c++17 -O2 gkmeans/gkmeans_exact.cpp -o tmp/gkmeans_exact")

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
            assert_run("echo data/#{data} | ./tmp/gkmeans > result/gkmeans_#{data}_#{i}")
            assert_run("echo data/#{data} | ./tmp/gkmeans_exact > result/gkmeans_exact_#{data}_#{i}")
        end
    end
end