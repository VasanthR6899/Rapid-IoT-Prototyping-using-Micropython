""" This class show cases the power of modules
"""

class Post:

    def __init__(self):
        self._titles = []

    def add_post(self, title):
        # This implementation forces the user to use the Post class as a list of string only
        self._titles.append(str(title))

    def delete_post(self, title):
        self._titles.remove(title)

    def get_post(self):
        return self._titles
