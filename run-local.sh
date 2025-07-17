#!/bin/bash

# Barvy pro výstup
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NC="\033[0m" # No Color

echo -e "${BLUE}=== ToDo App - Lokální spouštění ===${NC}\n"

case "$1" in
  "dev")
    echo -e "${GREEN}Spouštím vývojové prostředí s lokálním buildem...${NC}"
    docker compose -f docker-compose.dev.yaml up -d
    ;;
  "hot")
    echo -e "${GREEN}Spouštím vývojové prostředí s live reloadem...${NC}"
    docker compose -f docker-compose.dev-hot.yaml up
    ;;
  "ghcr" | "prod")
    echo -e "${GREEN}Spouštím produkční image z GHCR...${NC}"
    docker compose -f docker-compose.local.yaml up -d
    ;;
  "down")
    echo -e "${GREEN}Zastavuji všechny kontejnery...${NC}"
    docker compose -f docker-compose.dev.yaml -f docker-compose.dev-hot.yaml -f docker-compose.local.yaml down
    ;;
  "clean")
    echo -e "${GREEN}Zastavuji všechny kontejnery a mažu volume...${NC}"
    docker compose -f docker-compose.dev.yaml -f docker-compose.dev-hot.yaml -f docker-compose.local.yaml down -v
    ;;
  *)
    echo -e "${BLUE}Použití:${NC}"
    echo -e "  ./run-local.sh dev   ${GREEN}# Vývojové prostředí s lokálním buildem${NC}"
    echo -e "  ./run-local.sh hot   ${GREEN}# Vývojové prostředí s live reloadem${NC}"
    echo -e "  ./run-local.sh ghcr  ${GREEN}# Produkční image z GHCR${NC}"
    echo -e "  ./run-local.sh down  ${GREEN}# Zastavit všechny kontejnery${NC}"
    echo -e "  ./run-local.sh clean ${GREEN}# Zastavit všechny kontejnery a smazat volume${NC}"
    ;;
esac

if [ "$1" != "down" ] && [ "$1" != "clean" ] && [ "$1" != "" ]; then
  echo -e "\n${GREEN}Aplikace je dostupná na http://localhost${NC}"
  echo -e "${GREEN}Traefik dashboard je dostupný na http://localhost/dashboard/${NC}"
fi
