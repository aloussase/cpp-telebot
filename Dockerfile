FROM ubuntu

RUN apt-get update && apt-get install -y git build-essential cmake

WORKDIR /telebot

COPY . .

RUN apt-get install -y libssl-dev

RUN cmake -B build -S .
RUN cmake --build build

ENV BOT_TOKEN ""

CMD ["./build/telebot"]