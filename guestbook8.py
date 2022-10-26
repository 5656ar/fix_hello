from flask import Flask, render_template, request, redirect, url_for
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import Column, Integer, String, Date, CHAR, VARCHAR

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI']='postgresql://webadmin:BQEhbz67958@10.104.9.222:5432/testdb'
app.config['SQLALCHEMY_TRACK_MPDIFICATION'] = False


db = SQLAlchemy(app)

class Comments(db.Model):
    __tablename__ = 'Registration'
    id = Column(Integer, primary_key=True)
    student_id = Column(CHAR(13), nullable=False)
    subject_id = Column(VARCHAR(15), nullable=False)
    year = Column(CHAR(4), nullable=False)
    semester = Column(CHAR(1), nullable=False)
    grade = Column(CHAR(2))

@app.route('/')
def index():

    result = Comments.query.all()
    return render_template('index8.html', result=result)

@app.route('/sign')
def sign():
    return render_template('sign7.html')

@app.route('/process', methods=['POST'])
def process():

    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(host= '0.0.0.0',port= 80,debug=True)
    

#awd