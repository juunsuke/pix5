
print("YO", 12, 4.8, {}, "Proute", true)


function on_init()
	tex = Texture.load("data/konata.png")

	print(TextureFilter.Nearest)
end


function on_shutdown()
	tex:delete()
end


function on_frame()

	tex:draw(100, 10)

end




