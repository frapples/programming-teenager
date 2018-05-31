require "ex"

function main()
  local emacs_dir = os.currentdir()
  local root_dir = dirname(emacs_dir)


  add_to_environment_path(emacs_dir .. '\\bin')

  os.setenv("HOME", emacs_dir)
  local pid = os.spawn('runemacs.exe') --  Pid is nil means fail.
  local exitcode = pid:wait(pid)    -- Waiting process exit
end





function add_to_environment_path(path)
  local environment_path = os.getenv("PATH")
  -- 注意：windows中PATH的路径不能用引号包围
  os.setenv("PATH", path .. ';' .. environment_path)
end




function dirname(str)
  if str == '' then
     return str
    end

  local endchar = str:sub(str:len(), str:len())
  if endchar == '/' or endchar == '\\' then
     str = str:sub(1, str:len() - 1)
  end

	if str:match(".-[/|\\].-") then
    local name = string.gsub(str, "(.*)([/|\\].*)", "%1")
		return name
	else
		return ''
	end
end


main()