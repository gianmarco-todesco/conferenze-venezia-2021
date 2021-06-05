
def anagrams(word):
	m = len(word)
	if m == 1:
		yield word
	else:
		for s in anagrams(word[1:]):
			for i in range(m+1):
				yield s[:i] + word[:1] + s[i:]

for s in anagrams('image'): print(s)
