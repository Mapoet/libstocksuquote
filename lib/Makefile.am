lib_LIBRARIES = libstocks.a


if DEBUG
INCLUDES = -D__UNIX__ -DDEBUG
else
INCLUDES = -D__UNIX__
endif

libstocks_a_SOURCES = csv.c \
	 csv.h \
	 http.c \
	 http.h \
	 lists.c \
	 lists.h \
	 stocks.c \
	 stocks.h \
	 currency.c \
	history.c

include_HEADERS = \
	stocks.h

