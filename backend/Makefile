cleandocker:
	# Stops containers and removes containers, networks, volumes, and images created by up
	docker-compose -f docker-compose.yml down -v --remove-orphans


ifdef pv
	# Remove unused volumes
	docker volume ls -f name=$(MF_DOCKER_IMAGE_NAME_PREFIX) -f dangling=true -q | xargs -r docker volume rm
endif

install:
	cp ${BUILD_DIR}/* $(GOBIN)

run:
	docker-compose -f docker-compose.yml up
