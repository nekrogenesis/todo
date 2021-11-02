# TODO
![demo.png][demo.png]

## INSTALL
With ANSI-COLORS:
	
	make install-color

Without:

	make install

## UNINSTALL

	make uninstall

# DOCUMENTATION

	NAME

		todo - todo list tracker in C

	SYNOPSIS

		todo [options]

	OPTIONS

		i, init
			Creates a .todolist in the $HOME directory.

		a, add <string> ...

		c, check <entry> ...

		u, uncheck <entry> ...

		r, remove <entry> ...

		d, delete
			Deletes all checked entrys.

		e, edit <entry> <string>

		m, move <from entry> <to entry>

		h, help

	FILES

		$HOME/.todolist
