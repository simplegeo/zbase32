BUILD_NUMBER ?= 0000INVALID

.PHONY: zbase32/_version.py

version: zbase32/_version.py

zbase32/_version.py: zbase32/_version.py.m4
	m4 -D__BUILD__=$(BUILD_NUMBER) $^ > $@