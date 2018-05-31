from flask import Flask

app = Flask(__name__)


@app.route("/")
def hello():
    return "hello world!"


@app.route("/<id_>")
def show_id(id_):
    return id_


if __name__ == "__main__":
    app.run()
