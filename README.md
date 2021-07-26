This is a fork of SoapyRemote meant specifically for use with the XTRX as several things were broken. List of changes:
 - Set default MTU size to 4096 as the XTRX is hard coded to return 4096 when asked for MTU.
 - Set default window size to 64KiB, this needs to be a number which is a power of two. 

# Use any Soapy SDR remotely

## Build Status

- Travis: [![Travis Build Status](https://travis-ci.org/pothosware/SoapyRemote.svg?branch=master)](https://travis-ci.org/pothosware/SoapyRemote)

## Dependencies

* SoapySDR - https://github.com/pothosware/SoapySDR/wiki

## Documentation

* https://github.com/pothosware/SoapyRemote/wiki

## Licensing information

Use, modification and distribution is subject to the Boost Software
License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
