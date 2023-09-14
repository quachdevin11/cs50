import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # list of distinct stocks and share amount with placeholders for price and total value
    portfolio = db.execute("SELECT symbol, SUM(shares) as shares, '' as price, '' as value FROM history WHERE(id = 4) GROUP BY symbol HAVING SUM(shares) > 0")
    total = 0

    # steps through list and calcs out price and value
    for row in portfolio:
        price = lookup(row['symbol'])['price']
        row['price'] = usd(price)
        value = price * row['shares']
        row['value'] = usd(value)
        total = total + value

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cash[0]["cash"]
    total = total + cash

    return render_template("/index.html", portfolio=portfolio, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        symbol_info = lookup(symbol)

        if not symbol:
            return apology("Invalid Symbol", 400)
        elif symbol_info == None:
            return apology("Invalid Symbol", 400)
        elif not shares:
            return apology("Invalid Share Amount", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]

        if cash < symbol_info["price"]*float(shares):
            return apology("Insufficient Funds", 400)
        else:
            db.execute("INSERT INTO history VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)",session["user_id"], symbol_info["symbol"], shares, symbol_info["price"])
            balance = cash - symbol_info["price"]*float(shares)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])

            return redirect("/")


    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE id= ?", session["user_id"])
    return render_template("/history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        symbol_info = lookup(symbol)
        price = usd(symbol_info['price'])

        # Checking if Symbol is valid
        if symbol_info == None:
            return apology("Invalid Symbol", 400)

        return render_template("quoted.html", symbol_info=symbol_info, price=price)

    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Query to see if username already exists
        username_count = db.execute("SELECT COUNT(username) FROM users WHERE username = ?", username)
        username_count = username_count[0]["COUNT(username)"]

        if not username:
            return apology("Missing Username", 400)
        elif username_count > 0:
                return apology("Username Unavailable", 400)
        elif not password:
            return apology("Missing Password", 400)
        elif not confirmation:
            return apology("Missing Confirmation", 400)
        elif confirmation != password:
            return apology("Passwords Do Not Match", 400)

        hash = generate_password_hash(password)

        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)
        return redirect("/")

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        symbol_info = lookup(symbol)

        # Query of history where symbol was messed with
        symbols = db.execute("SELECT DISTINCT(symbol) FROM history WHERE symbol = ?", symbol)

        # Finding value of owned shares of symbol
        actual_shares = db.execute("SELECT SUM(shares) as shares FROM history WHERE symbol = ?", symbol)[0]['shares']

        if not symbol:
            return apology("Missing Symbol", 400)
        elif len(symbols) != 1:
            return apology("Stock Not Owned", 400)
        elif not shares:
            return apology("Missing Shares", 400)
        elif int(shares) > int(actual_shares) or int(shares) < 1:
            return apology("Insufficient Amount of Shares Owned", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]

        db.execute("INSERT INTO history VALUES (?, ?, -?, ?, CURRENT_TIMESTAMP)",session["user_id"], symbol, shares, symbol_info["price"])
        balance = cash + (symbol_info["price"]*float(shares))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])

        return redirect("/")

    else:
        symbols = db.execute("SELECT symbol, SUM(shares) as shares FROM history WHERE id = 4 GROUP BY symbol HAVING SUM(shares) > 0")
        for row in symbols:
            if row['shares'] == 0:
                symbols.remove(row)
        return render_template("/sell.html", symbols=symbols)


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Add Money to Balance"""
    if request.method == "POST":
        value = float(request.form.get("value"))

        # Checking if Symbol is valid
        if not value:
            return apology("Invalid Amount", 400)
        elif value < 0:
            return apology("Invalid Amount", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]

        balance = cash + value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])
        return render_template("added.html", value=usd(value))

    else:
        return render_template("add.html")

