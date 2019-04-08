# Сборка ---------------------------------------

# В качестве базового образа для сборки используем gcc:latest
FROM gcc:latest as build

# Установим рабочую директорию для сборки GoogleTest
WORKDIR /build

# Install libraries
RUN apt-get update && \
    apt-get install -y \
      php php-common \
      imagemagick \
      libsfml-dev \
      libsfml-graphics2.4 \
      libsfml-window2.4 \
      libsfml-system2.4 \
      cmake && \
      wget https://www.imagemagick.org/download/ImageMagick.tar.gz && \
      tar xf ImageMagick.tar.gz && \
      cd ImageMagick-7* && \
      ./configure && \
      make && \
      make install && \
      ldconfig /usr/local/lib

# Скопируем директорию /src в контейнер
ADD ./src /app/src
ADD ./res /app/src/build

# Установим рабочую директорию для сборки проекта
WORKDIR /app/build

# Выполним сборку нашего проекта, а также его тестирование
RUN cmake ../src && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . -o main -lpthread

# Запуск ---------------------------------------

# В качестве базового образа используем ubuntu:latest
FROM ubuntu:latest

# Добавим пользователя, потому как в Docker по умолчанию используется root
# Запускать незнакомое приложение под root'ом неприлично :)
RUN groupadd -r sample && useradd -r -g sample sample
USER sample

# Установим рабочую директорию нашего приложения
WORKDIR /app

# Скопируем приложение со сборочного контейнера в рабочую директорию
COPY --from=build /app/build/docker-220-graphics .

# Установим точку входа
ENTRYPOINT ["./220_graphics_app"]
