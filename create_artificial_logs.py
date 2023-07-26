import requests
from bs4 import BeautifulSoup


def main():
    base_url = "http://www.gutenberg.org/cache/epub/{book_id}/pg{book_id}.txt"
    r = requests.get("http://www.gutenberg.org/browse/scores/top")
    soup = BeautifulSoup(r.text)
    top_30 = {i.text: i['href'].split('/')[2]
              for i in soup.find_all('ol')[4].find_all('a')}

    with open("gutenberg_combined.txt", "a") as book_output:

        for title, link in top_30.items():
            book = requests.get(base_url.format(book_id=link))
            # with open(title, "w") as book_output:
            book_output.write(str(book.text))


if __name__ == "__main__":
    main()
