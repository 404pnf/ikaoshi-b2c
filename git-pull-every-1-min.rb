def every_n_sec n
  loop do
    before = Time.now
    yield
    interval = n - (Time.now - before)
    sleep(interval) if interval > 0
  end
end

every_n_sec(300) do
  system 'git pull'
  puts 'done git pulling'
end
