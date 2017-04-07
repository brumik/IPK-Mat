# NAME
ftrest - Restful File Trasport Client

# SYNOPSIS
ftrest COMMAND REMOTE-PATH \[LOCAL-PATH\]

# DESCRIPTION


# OPTIONS
* COMMAND
	* del Deletes file specified in REMOTE-PATH
	* get Copyes file from REMOTE-PATH to LOCAL-PATH
	* put Copyes file from LOCAL-PATH to REMOTE-PATH
	* lst Writes contents of directory in REMOTE-PATH
	* mkd Creates directory specified in REMOTE-PATH on server.
	* rmd Removes directory specified in REMOTE-PATH on server.
* REMOTE-PATH
	* Path to the server and the required directory. http://HOST:PORT/USER/PATH
* LOCAL PATH
	* Path to local file.

# FILES
* ftrestd
	* The responding server binary.
	
# ENVIRONMENT
Does not have specific environment.

# DIAGNOSTICS
* The following diagnostics may be issued on stderr:
* Not a directory.
	* When REMOTE-PATH is a file but used COMMAND lst or rmd.
* Directory not found.
	* When REMOTE-PATH points to directory which does not exists and used COMMAND lsr or rmd.
* Directory not empty.
	* When REMOTE-PATH points to directory which is not empty and used COMMAND rmd.
* Already exists.
	* When REMOTE-PATH points to directory which exists and used COMMAND mkd.
* Not a file.
	* When REMOTE-PATH points to directory but used COMMAND get or del.
* File not found.
	* When REMOTE-PATH points to non existing file and used COMMAND get or del.
* User Account Not Found
	* When operation is in non existing user account (or root).
* Internal error
	* When creating file at LOCAL-PATH fails at COMMAND get.
* Unknown error.
	* Other server side errors.
	
# Author
Levente Berky [xberky02@stud.fit.vutbr.cz](mailto:xberky02@stud.fit.vutbr.cz)

# SEE ALSO
ftrest(1)









# NAME
ftrestd - Restful File Trasport Server

# SYNOPSIS
ftrestd \[-r ROOT-FOLDER\] \[-p PORT\]

# DESCRIPTION


# OPTIONS
* -r ROOT-FOLDER
	* Specifies the server working directory. If not given uses cwd.
* -p PORT
	* Specifies the server port. If not given uses port 6677.

# FILES
* ftrest
	* The responding client binary.
	
# ENVIRONMENT
Filesystem required with user folders in it. With empty directory is useless.

# DIAGNOSTICS
* All errors are sent to client.
	
# Author
Levente Berky [xberky02@stud.fit.vutbr.cz](mailto:xberky02@stud.fit.vutbr.cz)

# SEE ALSO
ftrest(1)
