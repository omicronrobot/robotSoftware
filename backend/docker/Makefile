cleandocker:
	# Stops containers and removes containers, networks, volumes, and images created by up
	docker-compose -f docker-compose.yml down -v --remove-orphans

ifdef pv
	# Remove unused volumes
	docker volume ls -f name=$(OR_DOCKER_IMAGE_NAME_PREFIX) -f dangling=true -q | xargs -r docker volume rm
endif

stop:
	docker-compose -f addons/influxdb-reader/docker-compose.yml down
	docker-compose -f addons/influxdb-writer/docker-compose.yml down
	docker-compose -f addons/postgres-reader/docker-compose.yml down
	docker-compose -f addons/postgres-writer/docker-compose.yml down
	docker-compose -f docker-compose.yml down

startaddons:
	docker-compose -f addons/influxdb-reader/docker-compose.yml up -d
	docker-compose -f addons/influxdb-writer/docker-compose.yml up -d
	docker-compose -f addons/postgres-reader/docker-compose.yml up -d
	docker-compose -f addons/postgres-writer/docker-compose.yml up -d

run:
	docker-compose -f docker-compose.yml up

all:
	docker-compose -f docker-compose.yml up -d
	docker-compose -f addons/influxdb-reader/docker-compose.yml up -d
	docker-compose -f addons/influxdb-writer/docker-compose.yml up -d
	docker-compose -f addons/postgres-reader/docker-compose.yml up -d
	docker-compose -f addons/postgres-writer/docker-compose.yml up -d
