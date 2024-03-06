Lightweight technology of virtualization
- dosent use Hypervisor
- I want to only focus in the application

## docker image
- Base image (OS)
- build basic programs and drives on it (more images)

## concepts
- Dockerfile:
	- Plain text that contains docker commands
	- allows creation of image
- Docker Hub and Registries:	
	- Where to share the image as a repository
		- Harbor (open source)
		- Azure Container Registry
		- AWS
		- Artifact Registry

# Dockerfile
Example of running commands in docker and how to run a node shell command
```dockerfile
FROM node:17-alpine
WORKDIR /app
COPY package*.json .
RUN npm install --production
COPY . .
RUN npm run build
CMD ["node","dist/index.js"]
```

## Why docker
Its professional and may be mandatory
- **Reproducible Research**
- AI and Machine Learning
- *Field Study*
- Bioinformatics
- High performance computing
- *Scientific Simulations*
- **Collaboration**
- *Cloud Computing*
- Custom Software
- *Legacy Software Preservation*

# Running docker
`docker run hello-world` verify good instalation
`docker pull nginx` gets the image

```bash
docker run -d -p 80:80 -v $(pwd):/usr/share/nginx/html --name my_container nginx
```

- `-d` detach, 
- `-p 80:80` map port 80 of image to 80 port of system
- `-v $(pwd):/usr/share/nginx/html` mounts this file into the directory

___
# Basic usage
`docker images`
	see the available images

`docker inspect nginx`
	Gives me information about the image called `nginx`

`docker tag nginx my_custom_nginx:latest`
	Rename an image

`docker rmi nginx`
	Remove the docker image to free disk space

`docker history nginx`

## Manage Containers
`docker ps`
	Show all container running
	- add `-a` for all container, even stopped ones

`docker start <container_id_or_image>`
	Start and stopped cotainer

`docker stop <container_ir_or_image>`

`docker rm <container-id_or_name>`
	after stopping a container

`docker container prune`
	**Dangerous**
	Remove all stopped containers

`docker logs <container_id_name>`


___

