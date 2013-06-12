#! /usr/bin/env python
#-*- coding=utf-8 -*-
##
# \file
# script for modelconversion

from random import randint, choice
import sys
import re
from optparse import OptionParser

class Convertor():
	def __init__(self):
		#global variables
		self.vertices = []
		self.triangles = []
		self.quads = []
		self.cycle = []
		self.scale = [0, 0, 0]
		self.deltas = {}
		self.safe_title = ""
		self.base_name = ""
		self.bits = 0
		self.quad_strips = []

	def load(self, in_file):
		"""
			loads .raw file
		"""
		print "\tLoading file \"" + in_file + "\".";

		try:
			file = open(in_file,"r")
		except:
			print "\t\tFailed opening input file \"" + in_file + "\"."
			exit(1)

		self.safe_title = in_file.lower()
		self.safe_title = re.sub(".[^.]*$", "", self.safe_title)
		self.safe_title = re.sub("[ -.,;/]", "_", self.safe_title)

		self.base_name = re.sub(".[^.]*$", "", in_file)

		for line in file:
			shape = map(float, line.rsplit())

			if (len(shape) != 9 and len(shape) != 12):
				print "\t\tWrong file format."
				exit(1)


			# trangles
			if(len(shape) == 9):
				coord1 = [shape[0], shape[1], shape[2]]
				coord2 = [shape[3], shape[4], shape[5]]
				coord3 = [shape[6], shape[7], shape[8]]
				pos = len(self.vertices)
				self.vertices.extend([coord1, coord2, coord3])
				self.triangles.append([pos, pos + 1, pos + 2])

			#quads
			else:
				coord1 = [shape[0], shape[1], shape[2]]
				coord2 = [shape[3], shape[4], shape[5]]
				coord3 = [shape[6], shape[7], shape[8]]
				coord4 = [shape[9], shape[10], shape[11]]
				pos = len(self.vertices)
				self.vertices.extend([coord1, coord2, coord3, coord4])
				self.quads.append([pos, pos + 1, pos + 2, pos + 3])


	def roundCoords(self, accuracy):
		"""
				rounds coordinates to some accuracy
		"""
		print "\n\tRounding vertices to " + str(accuracy) + " decimal places"
		for i in xrange(len(self.vertices)):
			self.vertices[i][0] = round(self.vertices[i][0], accuracy)
			self.vertices[i][1] = round(self.vertices[i][1], accuracy)
			self.vertices[i][2] = round(self.vertices[i][2], accuracy)

	def unique(self, list):
		"""
			removes duplicates from list
		"""
		seen = {}
		result = []
		for item in list:
			if item in seen: continue
			seen[item] = 1
			result.append(item)
		return result

	def cleanup(self):
		"""
			removes duplicate vertices and degenerated triangles and quads.
			Creates cycle from minimum spanning tree and cuts it.
		"""
		print "\n\tStarting Cleanup. ",
		print str(len(self.quads)) + " quads and " + str(len(self.triangles)) + " triangles. Used " + str(len(self.vertices)) + " vertices"

		#remove duplicate vertices
		used = {}
		mapping = []
		new_vertices = []
		for vertex in self.vertices:
			index = used.setdefault(vertex[0], {}).setdefault(vertex[1], {}).setdefault(vertex[2], len(new_vertices))
			mapping.append(index)
			if(index == len(new_vertices)):
				new_vertices.append(vertex)

		self.vertices = new_vertices

		#fix indices
		self.quads = map(lambda x:map(mapping.__getitem__,x), self.quads)
		self.triangles = map(lambda x:map(mapping.__getitem__,x), self.triangles)


		#remove degenerated triangles and quads
		self.triangles = filter(lambda x:len(self.unique(x)) == 3, self.triangles)

		new_quads = []
		for quad in self.quads:
			unique = len(self.unique(quad))
			if (unique == 4):
				new_quads.append(quad)
			elif(unique < 3):
				pass
			else:
				for i in xrange(4):
					if (quad[i] == quad[(i + 1) % 4]):
						self.triangles.append([quad[(i + 1) % 4], quad[(i + 2) % 4], quad[(i + 3) % 4]])
						break

		self.quads = new_quads

		print "\tCleanup finished. ",
		print str(len(self.quads)) + " quads and " + str(len(self.triangles)) + " triangles. Used " + str(len(self.vertices)) + " vertices."


	def reorderVertices(self):
		"""
			reordering vertices - finds minimum spanning tree with kruskal's algorithm
		"""
		def distance(vert1, vert2):
#			return sqrt((vert2[0] - vert1[0])**2 + (vert2[1] - vert1[1])**2 + (vert2[2] - vert1[2])**2)
			return max(abs(vert2[0] - vert1[0]), abs(vert2[1] - vert1[1]), abs(vert2[2] - vert1[2]))

		print "\n\tReordering vertices."
		print "\t\tEnumerating edges"
		print "\t\t\t(uses a lot of time and memory)";

		edges = []

		for vertex in xrange(len(self.vertices)):
			for other in xrange(vertex + 1, len(self.vertices)):
				edges.append((distance(self.vertices[vertex], self.vertices[other]), vertex, other))
		edges.sort()

		#create minimal tree
		print "\t\tConstructing minimal spanning tree."
		minimal_tree = {}
		components = range(len(self.vertices))
		for edge in edges:
			if (components[edge[1]] != components[edge[2]]):
				components = map(lambda x : (components[edge[2]] if x == components[edge[1]] else x), components)
				minimal_tree.setdefault(edge[1], []).append(edge[2])
				minimal_tree.setdefault(edge[2], []).append(edge[1])

#		print "\t\t\tMinimal spanning tree: " + str(minimal_tree)

		# create cycle from tree
		print "\t\tConverting to Hamiltonian cycle."
		current_vertex = 0
		cycle = []
		visited = {}
		while(len(cycle) < len(self.vertices)):
			if (not visited.has_key(current_vertex)):
#				print "\t\t\tappending " + str(current_vertex)
				cycle.append(current_vertex)
			visited[current_vertex] = True
			for i in xrange(len(minimal_tree[current_vertex])):
				if (visited.has_key(minimal_tree[current_vertex][i])):
					end = False
					continue
				else:
					current_vertex = minimal_tree[current_vertex].pop(i)
					end = True
					break
			if (not end):
				current_vertex = minimal_tree[current_vertex].pop() #poping longest edge


		# fix indices and vertices
		cut = max(xrange(len(cycle)), key=lambda x: distance(self.vertices[cycle[x]], self.vertices[cycle[(x + 1) % len(cycle)]]))
		cycle = map(lambda x: cycle[(x + cut + 1) % len(cycle)], xrange(len(cycle)))
		self.cycle = cycle

		self.vertices = map(self.vertices.__getitem__, cycle)
		mapping = range(len(cycle))

		for c in range(len(cycle)):
			mapping[cycle[c]] = c

		self.quads = map(lambda x:map(mapping.__getitem__,x), self.quads)
		self.triangles = map(lambda x:map(mapping.__getitem__,x), self.triangles)

	def genDeltas(self):
		print "\n\tGenerating deltas."

		max_value = (2 ** self.bits) / 2 - 1
		min_value = -max_value - 1

		print "\t\tPicking deltas."
		self.deltas = range(len(self.vertices) - 1)
		for i in xrange(len(self.vertices) - 1):
			delta = [0, 0, 0]
			for coord in [0,1,2]:
				delta[coord] = self.vertices[i + 1][coord] - self.vertices[i][coord]
#				self.freq.setdefault(delta[coord], 0)
#				self.freq[delta[coord]] += 1
			self.deltas[i] = delta

		print "\t\tScaling deltas."
		eps = 0.000001
		for coord in [0, 1, 2]:
			min_delta = -eps
			max_delta = eps
			for delta in self.deltas:
				if (delta[coord] < min_delta):
					min_delta = delta[coord]
				elif (delta[coord] > max_delta):
					max_delta = delta[coord]

			self.scale[coord] = min(((max_value + 0.5) / max_delta), ((min_value - 0.5) / min_delta))
		print "\t\t\tResolution is [" + str(round(1 / self.scale[0], 5)) + ", " + str(round(1 / self.scale[1], 5)) + ", " + str(round(1 / self.scale[2], 5)) + "]"

		for coord in [0, 1, 2]:
			for i in xrange(len(self.deltas)):
				self.deltas[i][coord] = int(self.scale[coord] * self.deltas[i][coord])



	def reduceDeltas(self, accuracy):
		print "\n\tReducing deltas."

		freq = {}
		for coord in [0,1,2]:
			for i in xrange(len(self.deltas)):
				freq.setdefault(self.deltas[i][coord], 0)
				freq[self.deltas[i][coord]] += 1


		for coord in [0, 1, 2]:
			real = self.vertices[0][coord]
			for i in xrange(len(self.vertices) - 1):
				#sort freq
				keylist = freq.keys()
				keylist.sort(key = freq.__getitem__, reverse = True)

				dist_float = self.deltas[i][coord] / self.scale[coord]
				real += dist_float

				min_delta = accuracy
				max_delta = accuracy
				real_copy = real


				for j in xrange(i + 1, len(self.vertices) - 1):
					max_delta = min(max_delta, accuracy + self.vertices[j][coord] - real_copy)
					min_delta = min(min_delta, accuracy - self.vertices[j][coord] + real_copy)
					assert max_delta + min_delta <= 2 * accuracy
					real_copy += self.deltas[j][coord] / self.scale[coord]
				max_delta = (dist_float + max_delta) * self.scale[coord]
				min_delta = (dist_float - min_delta) * self.scale[coord]

				best = None
				for key in keylist:
					if (key <= max_delta and key >= min_delta):
						best = key
						break


				if (best != None and freq[best] >= freq[self.deltas[i][coord]] and best != self.deltas[i][coord] ):
					assert best <= max_delta and best >= min_delta
#					assert (max_delta - min_delta) <= 2 * accuracy * self.scale[coord]
					self.deltas[i][coord] = best
					real += (best / self.scale[coord]) - dist_float

					freq[best] += 1
					freq[self.deltas[i][coord]] -= 1
			

	def updateVertices(self):
		"""
			change vertices according to deltas, calculates errors
		"""
		print "\n\tUpdating vertices"
		maximum = [0, 0, 0]
		for coord in [0, 1, 2]:
			real = self.vertices[0][coord]
			for i in xrange(1, len(self.vertices)):
				real += self.deltas[i - 1][coord] / self.scale[coord]
				tmp = abs(self.vertices[i][coord] - real)
				if (maximum[coord] < tmp):
					maximum[coord] = tmp

				self.vertices[i][coord] = real

		print "\t\tMaximal displacement of vertices: [" + str(round(maximum[0], 5)) + ", " + str(round(maximum[1], 5)) + ", "+ str(round(maximum[2], 5)) + "]"


	def compressQuads(self, tries):
		print "\n\tCreating quad strips."

		edges = {}

		merged = self.quads
		merged.extend(self.triangles)

		for i in xrange(len(merged)):
			for j in xrange(len(merged[i])):
				edges.setdefault(merged[i][j], {})[merged[i][(j + 1) % len(merged[i])]] = (i, j) # might overwrite existing (non manifold)

		best_strips = None
		print "\t\tTrying to find optimal strips."
		for x in xrange(tries):
			if (x % 10 == 0 and best_strips != None):
				print "\r\t\t\t", x, "/" , str(tries) + ",", "best count of strips =", str(len(best_strips)),
				sys.stdout.flush()
			strips = []
			visited = map(lambda x: False, merged)
			while (True):
				available = filter(lambda x: x[0], map(lambda x: (not visited[x], x), range(len(visited))))
				if (len(available) == 0):
					break
				quad = choice(available)[1] # this is really a quad or a triangle
				side = randint(0, len(merged[quad]) - 1)

				visited[quad] = True
#				side = (side + 2) % 4

				strip = [];

				if (len(merged[quad]) == 3):
					add = randint(2, 3);
					strip = [merged[quad][side], merged[quad][(side + 1) % 3], merged[quad][(side + add) % 3], merged[quad][(side + add + 2) % 3]]
					# creates a degenerate quad, the strip is either [0, 1, 2, 1], or [0, 1, 0, 2]
				else:
					strip = [merged[quad][side], merged[quad][(side + 1) % 4], merged[quad][(side + 3) % 4], merged[quad][(side + 2) % 4]]
				
				while (True):
					try:
						(quad, side) = edges[strip[-2]][strip[-1]]
					except:
						break
					if (visited[quad]):
						break

					if (len(merged[quad]) == 3):
						add = randint(2, 3);
						strip.append(merged[quad][(side + add) % 3])
						strip.append(merged[quad][(side + add + 2) % 3])
					else:
						strip.append(merged[quad][(side + 3) % 4])
						strip.append(merged[quad][(side + 2) % 4])
					visited[quad] = True
				strips.append(strip)

			if (best_strips == None or len(strips) < len(best_strips)):
				best_strips = strips
				print ""
		print "\r\t\t\tdone                                                     "

		quad_strips_count = 0
		for strip in best_strips:
			quad_strips_count += len(strip)
		quad_strips_count += len(best_strips) - 1


		print "\t\tStrips count =", len(best_strips)
#		print "\t\tquad_strips_count =", quad_strips_count

		print "\t\tImprovement:", round(100 - 100 * (quad_strips_count + len(best_strips) - 1) / (4.0 * len(self.quads) + 3 * len(self.triangles)), 2), "%"

		self.quad_strips = best_strips


	def output(self):
		"""
			writing result to h and cfile
		"""
		h_file = self.base_name +"_generated.h"
		c_file = self.base_name +"_generated.c"
		print "\n\tWriting files " + h_file + ", " + c_file + "."



		hout = open(h_file, "w")
		print >> hout, """\
/** @file
This file was automaticaly generated with raw2c convertor.
Please do not edit if you have no clue, what the hell is here. */
#ifndef _""" + self.safe_title.upper() + """_H_
#define _""" + self.safe_title.upper() + """_H_

#include <stdint.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include \"framework/meshmanager/meshmanager.h\"

#define UNPACK_""" + self.safe_title.upper() + """(model)\\
	unpackModel(model_""" + self.safe_title + """_vertices,\\
		model_""" + self.safe_title + """_vertexCount,\\
		model_""" + self.safe_title + """_vertex0,\\
		model_""" + self.safe_title + """_vertexScale,\\
		model_""" + self.safe_title + """_quadsCount,\\
		model_""" + self.safe_title + """_quads)
		
// vertices
extern""",
		if (self.bits == 8):
			print >> hout, "int8_t",
		elif (self.bits == 16):
			print >> hout, "int16_t",
		else:
			print >> hout, "int32_t",
				
		print >> hout, """model_""" + self.safe_title + """_vertices[];
extern uint16_t model_""" + self.safe_title + """_vertexCount;

extern GLfloat model_""" + self.safe_title + """_vertex0[];
extern GLfloat model_""" + self.safe_title + """_vertexScale[];

// indices
extern uint16_t model_""" + self.safe_title + """_quadsCount;
extern uint16_t model_""" + self.safe_title + """_quads[];


#endif
"""
		hout.close()




		cout = open(c_file, "w")
		print >> cout, """\
/** @file
This file was automaticaly generated with raw2c convertor.
Please do not edit if you have no clue, what the hell is here. */

#include \"""" + h_file + """\"

""",
		if (self.bits == 8):
			print >> cout, "int8_t",
		elif (self.bits == 16):
			print >> cout, "int16_t",
		else:
			print >> cout, "int32_t",
			
		print >> cout, """model_""" + self.safe_title + """_vertices[] = {"""
		for delta in self.deltas:
			print >> cout, "\t",
			for coord in [0, 1, 2]:
				print >> cout, str(delta[coord]) + ", ",
			print >> cout

		print >> cout, """};
uint16_t model_""" + self.safe_title + """_vertexCount = """ + str(len(self.vertices)) + """;

GLfloat model_""" + self.safe_title + """_vertex0[] = {""" + str(self.vertices[0][0]) + ", " + str(self.vertices[0][1]) + ", " + str(self.vertices[0][2]) + """};
GLfloat model_""" + self.safe_title + """_vertexScale[] = {""" + str(1 / self.scale[0]) + ", " + str(1 / self.scale[1]) + ", " + str(1 / self.scale[2]) + """};


uint16_t model_""" + self.safe_title + """_quads[] = {"""
		for strip in self.quad_strips:
			print >> cout, "\t",
			for index in strip:
				print >> cout, str(index) + ", ",
			if (strip == self.quad_strips[-1]):
				print >> cout, "\n",
			else:
				print >> cout, "STRIPSEPARATOR,\n",

		print >> cout, """};
				
uint16_t model_""" + self.safe_title + """_trianglesCount = """ + str(len(self.triangles)) + """;
uint16_t model_""" + self.safe_title + """_quadsCount = """ + str(len(self.quads)) + """;



"""


	def outputRaw(self):
		"""
			writing result into raw file
		"""
		out_raw_file = self.base_name + "_generated.raw"
		print "\n\tWriting files \"" + out_raw_file + "\".";

		try:
			file = open(out_raw_file, "w")
		except:
			print "\tError writing output file \"" + out_raw_file + "\"."
			exit(1)

		def printObject(obj):
			for i in obj:
				for coord in self.vertices[i]:
					file.write(str(coord) + " ")
			file.write("\n")

		map(printObject, self.triangles)
		map(printObject, self.quads)



	def decompress(self):
		out_raw_file = self.base_name + "_generated.raw"
		print "\n\tWriting files \"" + out_raw_file + "\".";

		try:
			file = open(out_raw_file, "w")
		except:
			print "\tError writing output file \"" + out_raw_file + "\"."
			exit(1)

		for triangle in self.triangles:
				for index in [0, 1, 2]:
					for coord in [0, 1, 2]:
						print >> file, self.vertices[triangle[index]][coord],
				print >> file, "\n",



		for strip in self.quad_strips:
			for i in xrange(0, len(strip) - 3, 2):
				for quad in [0, 1, 2, 3]:
					for coord in [0, 1, 2]:
						if (quad == 3):
							print >> file, self.vertices[strip[i + 2]][coord],
						elif (quad == 2):
							print >> file, self.vertices[strip[i + 3]][coord],
						else:
							print >> file, self.vertices[strip[i + quad]][coord],
				print >> file, "\n",




if __name__ == "__main__":
	# parse options
	usage = "usage: %prog [options] raw_file"
	parser = OptionParser(usage=usage)
	parser.add_option("-a", "--accuracy", dest="accuracy", type="float", help="Determines allowed error for each vertex", metavar="<ACCURACY>")
	parser.add_option("-b", "--bits", dest="bits", type="int", help="How many bits will be used for each coord", metavar="<BITS>")
	parser.add_option("-r", "--round", dest="round", type="int", help="Round original vertices coords on <ROUND> decimal places", metavar="<ROUND>")
	parser.add_option("-f", "--file", dest="in_file", help="Target .raw file to convert", metavar="<FILE>")
	parser.add_option("-i", "--iterations", dest="iter",type="int", help="Number of tries to find best strips.", metavar="<COUNT>")
	(options, args) = parser.parse_args()

	if(not options.in_file):
		print "Option -f not specified."
		exit(1)

	if(not options.accuracy):
		print "Option -a not specified. Using 0.1."
		options.accuracy = 0.1
		
	if(not options.round):
		print "Option -r not specified. Using 4."
		options.round = 4
		
	if(not options.iter):
		print "Option -i not specified. Using 1000."
		options.iter = 1000

	if(options.bits):
		if(options.bits != 8 and options.bits != 16 and options.bits != 32):
			print "Bits must be 8, 16 or 32"
			exit(1)
	else:
		print "Option -b not specified. Using 16."
		options.bits = 16

	convertor = Convertor()
	convertor.bits = options.bits

	convertor.load(options.in_file)
	convertor.roundCoords(options.round)
	convertor.cleanup()
	convertor.reorderVertices()
	convertor.genDeltas()
	convertor.reduceDeltas(options.accuracy)
	convertor.updateVertices()

	convertor.compressQuads(options.iter)

	convertor.output()
#	convertor.decompress()
#	convertor.outputRaw()
