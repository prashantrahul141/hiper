.PHONY: hiper guest

all: hiper guest

hiper:
	@printf "building hiper..........\n"
	cd ./src && make

guest:
	@printf "\n\nbuilding guest..........\n"
	cd ./guest/ && make

clean:
	rm -rf build
