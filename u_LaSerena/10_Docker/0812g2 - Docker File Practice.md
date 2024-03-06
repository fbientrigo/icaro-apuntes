We can add to the docker file a command for when it instarts
```dockerfile
RUN pip install pandas matplotlib
CMD ["jupyter","notebook"]
```

- All Dockerfile starts with base image
`FROM jupyter/minimal-notebook`

- Environment configuration
`END NODE_ENV=production`

- Copying files
`COPY . /app`

- Running commands, install packages and many
`RUN npm install`

- Exposing ports
`EXPOSE 8889`
	- indicates that the container listens that port

- Container Execution Command or Entry point
`CMD ["node","app.js"]`
	Command when the container starts

___
We run this in command line

- Build the image
`docker build -t jupyter-server .`
	This will create a docker image

- Running the image
`docker run -p 8889:8889 jupyter-server`
	we use an exposed port

