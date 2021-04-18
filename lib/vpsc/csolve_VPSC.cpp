/**
 * \brief Bridge for C programs to access solve_VPSC (which is in C++)
 *
 * Authors:
 *   Tim Dwyer <tgdwyer@gmail.com>
 *
 * Copyright (C) 2005 Authors
 *
 * This version is released under the CPL (Common Public License) with
 * the Graphviz distribution.
 * A version is also available under the LGPL as part of the Adaptagrams
 * project: https://github.com/mjwybrow/adaptagrams.  
 * If you make improvements or bug fixes to this code it would be much
 * appreciated if you could also contribute those changes back to the
 * Adaptagrams repository.
 */


#include <iostream>
#include <vpsc/variable.h>
#include <vpsc/constraint.h>
#include <vpsc/generate-constraints.h>
#include <vpsc/solve_VPSC.h>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <vpsc/csolve_VPSC.h>
Variable* newVariable(int id, double desiredPos, double weight) {
	return new Variable(id,desiredPos,weight);
}
Constraint* newConstraint(Variable* left, Variable* right, double gap) {
	return new Constraint(left,right,gap);
}
VPSC* newVPSC(int n, Variable* vs[], int m, Constraint* cs[]) {
	return new VPSC(n,vs,m,cs);
}
VPSC* newIncVPSC(int n, Variable* vs[], int m, Constraint* cs[]) {
	return new IncVPSC(n,vs,m,cs);
}

int genXConstraints(int n, boxf* bb, Variable** vs, Constraint*** cs,int transitiveClosure) {
	std::vector<Rectangle*> rs(n);
	for(int i=0;i<n;i++) {
		rs[i]=new Rectangle(bb[i].LL.x,bb[i].UR.x,bb[i].LL.y,bb[i].UR.y);
	}
	int m = generateXConstraints(n,rs.data(),vs,*cs,transitiveClosure?true:false);
	for(int i=0;i<n;i++) {
		delete rs[i];
	}
	return m;
}
int genYConstraints(int n, boxf* bb, Variable** vs, Constraint*** cs) {
	std::vector<Rectangle*> rs(n);
	for(int i=0;i<n;i++) {
		rs[i]=new Rectangle(bb[i].LL.x,bb[i].UR.x,bb[i].LL.y,bb[i].UR.y);
	}
	int m = generateYConstraints(n,rs.data(),vs,*cs);
	for(int i=0;i<n;i++) {
		delete rs[i];
	}
	return m;
}

Constraint** newConstraints(int m) {
	return new Constraint*[m];
}
void deleteConstraints(int m, Constraint **cs) {
	for(int i=0;i<m;i++) {
		delete cs[i];
	}
	delete [] cs;
}
void deleteConstraint(Constraint* c) {
	delete c;
}
void deleteVariable(Variable* v) {
	delete v;
}
void satisfyVPSC(VPSC* vpsc) {
	try {
		vpsc->satisfy();
	} catch(const char *e) {
		std::cerr << e << "\n";
		std::exit(1);
	}
}
int getSplitCnt(IncVPSC *vpsc) {
	return vpsc->splitCnt;
}
void deleteVPSC(VPSC *vpsc) {
	assert(vpsc!=nullptr);
	delete vpsc;
}
void solveVPSC(VPSC* vpsc) {
	vpsc->solve();
}
void splitIncVPSC(IncVPSC* vpsc) {
	vpsc->splitBlocks();
}
void setVariableDesiredPos(Variable *v, double desiredPos) {
	v->desiredPosition = desiredPos;
}
double getVariablePos(Variable *v) {
	return v->position();
}
void remapInConstraints(Variable *u, Variable *v, double dgap) {
	for(Constraints::iterator i=u->in.begin();i!=u->in.end();i++) {
		Constraint* c=*i;	
		c->right=v;
		c->gap+=dgap;
		v->in.push_back(c);
	}
	u->in.clear();
}
void remapOutConstraints(Variable *u, Variable *v, double dgap) {
	for(Constraints::iterator i=u->out.begin();i!=u->out.end();i++) {
		Constraint* c=*i;	
		c->left=v;
		c->gap+=dgap;
		v->out.push_back(c);
	}
	u->out.clear();
}
int getLeftVarID(Constraint *c) {
	return c->left->id;
}
int getRightVarID(Constraint *c){
	return c->right->id;
}
double getSeparation(Constraint *c){
	return c->gap;
}
