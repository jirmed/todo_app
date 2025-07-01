# Build stage
FROM node:20-alpine AS build

WORKDIR /app
COPY . .
RUN npm install
RUN npm run build

# Production nginx stage
FROM nginx:alpine

# Zkopíruj vybuilděnou aplikaci do nginx html složky
COPY --from=build /app/dist /usr/share/nginx/html

# (volitelné) Pokud chceš vlastní nginx.conf (např. SPA fallback, proxy), přidej:
# COPY nginx.conf /etc/nginx/conf.d/default.conf

EXPOSE 80
