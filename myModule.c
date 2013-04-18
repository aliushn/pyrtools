#include <Python.h>
#include "Numeric/arrayobject.h"
#include "convolve.h"
/* compile with: gcc -shared -I/usr/include/python2.6/ -lpython2.6 -o myModule.so -fPIC myModule.c */

#define notDblMtx(it) (!mxIsNumeric(it) || !mxIsDouble(it) || mxIsSparse(it) || mxIsComplex(it))


static PyObject* py_myFunction(PyObject* self, PyObject* args)
//void mexFunction(int nlhs,	     /* Num return vals on lhs */
//		 mxArray *plhs[],    /* Matrices on lhs      */
//		 int nrhs,	     /* Num args on rhs    */
//		 const mxArray *prhs[]     /* Matrices on rhs */
//		 )
{
  //double *image,*filt, *temp, *result;
  int x_fdim, y_fdim;
  int Nargs;
  int x_idim, y_idim;
  //int x_rdim, y_rdim;
  //int x_start = 1;
  //int x_step = 1;
  //int y_start = 1;
  //int y_step = 1;
  //int x_stop, y_stop;
  //mxArray *arg;
  PyObject *arg1, *arg2;
  PyArrayObject *image, *filt;
  /*double *mxMat;
  char edges[15] = "reflect1";
  */
  
  // parse input parameters
  //if( !PyArg_ParseTuple(args, "iiiO", &Nargs, &x_idim, &y_idim, &arg1) )
  if( !PyArg_ParseTuple(args, "iiiOiiO", &Nargs, &x_idim, &y_idim, &arg1, 
			&x_fdim, &y_fdim, &arg2) )
    return NULL;
  image = (PyArrayObject *)PyArray_ContiguousFromObject(arg1, PyArray_INT, 1, 
							x_idim * y_idim);
  if(image == NULL)
    return NULL;
  if(image->nd != 2 || image->descr->type_num != PyArray_INT){
    PyErr_SetString(PyExc_ValueError, 
		    "array must be two-dimensional and of type int");
    return NULL;
  }

  filt = (PyArrayObject *)PyArray_ContiguousFromObject(arg2, PyArray_DOUBLE, 
						       //x_fdim * y_fdim, 1);
						       1, 5);
  if(filt == NULL)
    return NULL;
  if(filt->nd != 2 || filt->descr->type_num != PyArray_DOUBLE){
    PyErr_SetString(PyExc_ValueError, 
		    "array must be two-dimensional and of type int");
    return NULL;
  }

  return PyArray_Return(image);
  //return Py_BuildValue("s", "done");

  /*
  if (nrhs<2) mexErrMsgTxt("requres at least 2 args.");

  * ARG 1: IMAGE  *
  arg = prhs[0];
  if notDblMtx(arg) mexErrMsgTxt("IMAGE arg must be a non-sparse double float matrix.");
  image = mxGetPr(arg);
  x_idim = (int) mxGetM(arg); * X is inner index! *
  y_idim = (int) mxGetN(arg);

  * ARG 2: FILTER *
  arg = prhs[1];
  if notDblMtx(arg) mexErrMsgTxt("FILTER arg must be non-sparse double float matrix.");
  filt = mxGetPr(arg);
  x_fdim = (int) mxGetM(arg); 
  y_fdim = (int) mxGetN(arg);

  if ((x_fdim > x_idim) || (y_fdim > y_idim))
    {
    mexPrintf("Filter: [%d %d], Image: [%d %d]\n",x_fdim,y_fdim,x_idim,y_idim);
    mexErrMsgTxt("FILTER dimensions larger than IMAGE dimensions.");
    }

  * ARG 3 (optional): EDGES *
  if (nrhs>2) 
      {
      if (!mxIsChar(prhs[2]))
	mexErrMsgTxt("EDGES arg must be a string.");
      mxGetString(prhs[2],edges,15);
      }

  * ARG 4 (optional): STEP *
  if (nrhs>3)
      {
      arg = prhs[3];
      if notDblMtx(arg) mexErrMsgTxt("STEP arg must be a double float matrix.");
      if (mxGetM(arg) * mxGetN(arg) != 2)
    	 mexErrMsgTxt("STEP arg must contain two elements.");
      mxMat = mxGetPr(arg);
      x_step = (int) mxMat[0];
      y_step = (int) mxMat[1];
      if ((x_step<1) || (y_step<1))
         mexErrMsgTxt("STEP values must be greater than zero.");
      }

  * ARG 5 (optional): START *
  if (nrhs>4)
      {
      arg = prhs[4];
      if notDblMtx(arg) mexErrMsgTxt("START arg must be a double float matrix.");
      if (mxGetM(arg) * mxGetN(arg) != 2)
	mexErrMsgTxt("START arg must contain two elements.");
      mxMat = mxGetPr(arg);
      x_start = (int) mxMat[0];
      y_start = (int) mxMat[1];
      if ((x_start<1) || (x_start>x_idim) ||
          (y_start<1) || (y_start>y_idim))
         mexErrMsgTxt("START values must lie between 1 and the image dimensions.");
      }
  x_start--;  * convert from Matlab to standard C indexes *
  y_start--;

  * ARG 6 (optional): STOP *
  if (nrhs>5)
      {
      if notDblMtx(prhs[5]) mexErrMsgTxt("STOP arg must be double float matrix.");
      if (mxGetM(prhs[5]) * mxGetN(prhs[5]) != 2)
    	 mexErrMsgTxt("STOP arg must contain two elements.");
      mxMat = mxGetPr(prhs[5]);
      x_stop = (int) mxMat[0];
      y_stop = (int) mxMat[1];
      if ((x_stop<x_start) || (x_stop>x_idim) ||
          (y_stop<y_start) || (y_stop>y_idim))
         mexErrMsgTxt("STOP values must lie between START and the image dimensions.");
      }
  else
      {
      x_stop = x_idim;
      y_stop = y_idim;
      }
	  
  x_rdim = (x_stop-x_start+x_step-1) / x_step;
  y_rdim = (y_stop-y_start+y_step-1) / y_step;
  
  *  mxFreeMatrix(plhs[0]); *
  plhs[0] = (mxArray *) mxCreateDoubleMatrix(x_rdim,y_rdim,mxREAL);
  if (plhs[0] == NULL) mexErrMsgTxt("Cannot allocate result matrix");
  result = mxGetPr(plhs[0]);

  temp = mxCalloc(x_fdim*y_fdim, sizeof(double));
  if (temp == NULL)
    mexErrMsgTxt("Cannot allocate necessary temporary space");

  *
    printf("i(%d, %d), f(%d, %d), r(%d, %d), X(%d, %d, %d), Y(%d, %d, %d), %s\n",
	 x_idim,y_idim,x_fdim,y_fdim,x_rdim,y_rdim,
	 x_start,x_step,x_stop,y_start,y_step,y_stop,edges);
	 *

  if (strcmp(edges,"circular") == 0)
  	internal_wrap_reduce(image, x_idim, y_idim, filt, x_fdim, y_fdim,
			     x_start, x_step, x_stop, y_start, y_step, y_stop,
			     result);
  else internal_reduce(image, x_idim, y_idim, filt, temp, x_fdim, y_fdim,
		       x_start, x_step, x_stop, y_start, y_step, y_stop,
		       result, edges);

		       mxFree((char *) temp); */
  //return;
  //return Py_BuildValue("s", "done");
} 



/*
 * Function to be called from Python
 */
/*static PyObject* py_myFunction(PyObject* self, PyObject* args)
{
	char *s = "Hello from C!";
	return Py_BuildValue("s", s);
}*/

/*
 * Another function to be called from Python
 */
static PyObject* py_myOtherFunction(PyObject* self, PyObject* args)
{
	double x, y;
	PyArg_ParseTuple(args, "dd", &x, &y);
	return Py_BuildValue("d", x*y);
}

/*
 * Bind Python function names to our C functions
 */
static PyMethodDef myModule_methods[] = {
	{"myFunction", py_myFunction, METH_VARARGS},
	{"myOtherFunction", py_myOtherFunction, METH_VARARGS},
	{NULL, NULL}
};

/*
 * Python calls this to let us initialize our module
 */
void initmyModule()
{
	(void) Py_InitModule("myModule", myModule_methods);
	import_array();
}
