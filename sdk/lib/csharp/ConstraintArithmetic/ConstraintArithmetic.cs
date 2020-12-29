using System;
using System.Collections.Generic;
using static Solver;

#pragma warning disable CS0660, CS0661

namespace ConstraintArithmetic {
    public struct ArithmeticConstraint {
        public float LeftConstant;
        public float[] LeftCoefficients;

        public float RightConstant;
        public float[] RightCoefficients;

        public ArithmeticConstraint(float leftConstant, float[] leftCoefficients, float rightConstant, float[] rightCoefficients) {
            LeftConstant = leftConstant;
            LeftCoefficients = leftCoefficients;

            RightConstant = rightConstant;
            RightCoefficients = rightCoefficients;
        }
    }

    public enum ArithmeticInequality {
        Equal,
        LessThanOrEqual,
        GreaterThanOrEqual
    }

    public class ArithmeticContext {
        public List<bool> Variables = new List<bool>();

        public List<ArithmeticConstraint> Constraints = new List<ArithmeticConstraint>();

        public ArithmeticExpression Objective;

        public int[] SolutionVariableIndices;
        public float[] SolutionConstants;

        public static ArithmeticConstraint CreateConstraint(ArithmeticExpression left, ArithmeticInequality inequality, ArithmeticExpression right) {
            var context = left.Context;

            switch(inequality) {
                case ArithmeticInequality.Equal: break;

                case ArithmeticInequality.LessThanOrEqual: {
                    var slackVariable = context.Variables.Count;

                    context.Variables.Add(false);

                    Array.Resize(ref left.Coefficients, context.Variables.Count);

                    left.Coefficients[slackVariable] = 1.0f;
                } break;

                case ArithmeticInequality.GreaterThanOrEqual: {
                    var slackVariable = context.Variables.Count;

                    context.Variables.Add(false);

                    Array.Resize(ref left.Coefficients, context.Variables.Count);

                    left.Coefficients[slackVariable] = -1.0f;
                } break;
            }

            return new ArithmeticConstraint(left.Constant, left.Coefficients, right.Constant, right.Coefficients);
        }

        public void AddConstraints(params ArithmeticConstraint[] constraints) {
            Constraints.AddRange(constraints);
        }

        public void Solve() {
            // Create full-sized tableau
            var objectiveConstant = Objective.Constant;
            var objectiveCoefficients = new float[Variables.Count];

            if(Objective.Coefficients != null) {
                for(var i = 0; i < Objective.Coefficients.Length; i += 1) {
                    objectiveCoefficients[i] = Objective.Coefficients[i];
                }
            }

            var constraintConstants = new float[Constraints.Count];

            var constraintCoefficients = new float[Constraints.Count * Variables.Count];

            for(var i = 0; i < Constraints.Count; i += 1) {
                var constraint = Constraints[i];

                constraintConstants[i] = constraint.LeftConstant;
                constraintConstants[i] -= constraint.RightConstant;

                if(constraint.LeftCoefficients != null) {
                    for(var j = 0; j < constraint.LeftCoefficients.Length; j += 1) {
                        constraintCoefficients[i * Variables.Count + j] = constraint.LeftCoefficients[j];
                    }
                }

                if(constraint.RightCoefficients != null) {
                    for(var j = 0; j < constraint.RightCoefficients.Length; j += 1) {
                        constraintCoefficients[i * Variables.Count + j] -= constraint.RightCoefficients[j];
                    }
                }
            }

            var constraintVariableIndices = new UIntPtr[Constraints.Count];

            // Run solver
            solve(
                (UIntPtr)Variables.Count,
                (UIntPtr)Constraints.Count,
                Variables.ToArray(),
                ref objectiveConstant,
                objectiveCoefficients,
                constraintVariableIndices,
                constraintConstants,
                constraintCoefficients
            );

            // Extract variable values from solution
            SolutionVariableIndices = new int[Constraints.Count];

            for(var i = 0; i < Constraints.Count; i += 1) {
                SolutionVariableIndices[i] = (int)constraintVariableIndices[i];
            }

            SolutionConstants = constraintConstants;
        }

        public void Minimize(ArithmeticExpression expression) {
            Objective = Objective + expression;
        }

        public void Maximize(ArithmeticExpression expression) {
            Objective = Objective - expression;
        }

        public void Minimize(ArithmeticTerm term) {
            Minimize(term);
        }

        public void Maximize(ArithmeticTerm term) {
            Maximize(term);
        }
    }

    public struct ArithmeticExpression {
        public ArithmeticContext Context;

        public float Constant;

        public float[] Coefficients;

        public ArithmeticExpression(ArithmeticContext context, float constant) {
            Context = context;
            Constant = constant;
            Coefficients = null;
        }

        public ArithmeticExpression(ArithmeticContext context, float[] coefficients) {
            Context = context;
            Constant = 0.0f;
            Coefficients = coefficients;
        }

        public ArithmeticExpression(ArithmeticContext context, float constant, float[] coefficients) {
            Context = context;
            Constant = constant;
            Coefficients = coefficients;
        }

        // Expression-Expression operators

        public static ArithmeticExpression operator +(ArithmeticExpression a, ArithmeticExpression b) {
            var Context = a.Context;

            float[] coefficients;
            if(a.Coefficients.Length > b.Coefficients.Length) {
                for(var i = 0; i < a.Coefficients.Length; i += 1) {
                    a.Coefficients[i] += b.Coefficients[i];
                }

                coefficients = a.Coefficients;
            } else {
                for(var i = 0; i < b.Coefficients.Length; i += 1) {
                    b.Coefficients[i] += a.Coefficients[i];
                }

                coefficients = b.Coefficients;
            }

            return new ArithmeticExpression(Context, a.Constant + b.Constant, coefficients);
        }

        public static ArithmeticExpression operator -(ArithmeticExpression expression) {
            expression.Constant = -expression.Constant;

            for(var i = 0; i < expression.Coefficients.Length; i += 1) {
                expression.Coefficients[i] = -expression.Coefficients[i];
            }

            return expression;
        }

        public static ArithmeticExpression operator -(ArithmeticExpression a, ArithmeticExpression b) {
            return a + -b;
        }

        public static ArithmeticConstraint operator ==(ArithmeticExpression left, ArithmeticExpression right) {
            return ArithmeticContext.CreateConstraint(left, ArithmeticInequality.Equal, right);
        }

        public static ArithmeticConstraint operator !=(ArithmeticExpression left, ArithmeticExpression right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator <=(ArithmeticExpression left, ArithmeticExpression right) {
            return ArithmeticContext.CreateConstraint(left, ArithmeticInequality.LessThanOrEqual, right);
        }

        public static ArithmeticConstraint operator >=(ArithmeticExpression left, ArithmeticExpression right) {
            return ArithmeticContext.CreateConstraint(left, ArithmeticInequality.GreaterThanOrEqual, right);
        }

        // Expression-float operators

        public static ArithmeticExpression operator +(ArithmeticExpression expression, float constant) {
            expression.Constant += constant;

            return expression;
        }

        public static ArithmeticExpression operator +(float constant, ArithmeticExpression expression) {
            return expression + constant;
        }

        public static ArithmeticExpression operator -(ArithmeticExpression expression, float constant) {
            return expression + -constant;
        }

        public static ArithmeticExpression operator -(float constant, ArithmeticExpression expression) {
            return expression + -constant;
        }

        public static ArithmeticExpression operator *(ArithmeticExpression expression, float constant) {
            expression.Constant *= constant;

            for(var i = 0; i < expression.Coefficients.Length; i += 1) {
                expression.Coefficients[i] *= constant;
            }

            return expression;
        }

        public static ArithmeticExpression operator *(float constant, ArithmeticExpression expression) {
            return expression * constant;
        }

        public static ArithmeticExpression operator /(ArithmeticExpression expression, float constant) {
            return expression * (1.0f / constant);
        }

        public static ArithmeticConstraint operator ==(ArithmeticExpression left, float right) {
            var rightExpression = new ArithmeticExpression(left.Context, right);

            return left == rightExpression;
        }

        public static ArithmeticConstraint operator ==(float left, ArithmeticExpression right) {
            var leftExpression = new ArithmeticExpression(right.Context, left);

            return leftExpression == right;
        }

        public static ArithmeticConstraint operator !=(ArithmeticExpression left, float right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator !=(float left, ArithmeticExpression right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator <=(ArithmeticExpression left, float right) {
            var rightExpression = new ArithmeticExpression(left.Context, right);

            return left <= rightExpression;
        }

        public static ArithmeticConstraint operator <=(float left, ArithmeticExpression right) {
            return right >= left;
        }

        public static ArithmeticConstraint operator >=(ArithmeticExpression left, float right) {
            var rightExpression = new ArithmeticExpression(left.Context, right);

            return left >= rightExpression;
        }

        public static ArithmeticConstraint operator >=(float left, ArithmeticExpression right) {
            return right <= left;
        }
    }

    public struct ArithmeticTerm {
        public ArithmeticContext Context;

        public int VariableIndex;

        public float Coefficient;

        public ArithmeticTerm(ArithmeticContext context, int variableIndex, float coefficient) {
            Context = context;
            VariableIndex = variableIndex;
            Coefficient = coefficient;
        }

        // Term-Term operators

        public static ArithmeticExpression operator +(ArithmeticTerm a, ArithmeticTerm b) {
            var context = a.Context;

            var coefficients = new float[context.Variables.Count];

            coefficients[a.VariableIndex] = a.Coefficient;
            coefficients[b.VariableIndex] += b.Coefficient;

            return new ArithmeticExpression(context, coefficients);
        }

        public static ArithmeticTerm operator-(ArithmeticTerm term) {
            term.Coefficient = -term.Coefficient;

            return term;
        }

        public static ArithmeticExpression operator -(ArithmeticTerm a, ArithmeticTerm b) {
            return a + -b;
        }

        public static ArithmeticConstraint operator ==(ArithmeticTerm left, ArithmeticTerm right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = (ArithmeticExpression)right;

            return leftExpression == rightExpression;
        }

        public static ArithmeticConstraint operator !=(ArithmeticTerm left, ArithmeticTerm right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator <=(ArithmeticTerm left, ArithmeticTerm right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = (ArithmeticExpression)right;

            return leftExpression <= rightExpression;
        }

        public static ArithmeticConstraint operator >=(ArithmeticTerm left, ArithmeticTerm right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = (ArithmeticExpression)right;

            return leftExpression >= rightExpression;
        }

        // Term-float operators

        public static ArithmeticExpression operator +(ArithmeticTerm term, float constant) {
            var context = term.Context;

            var coefficients = new float[context.Variables.Count];

            coefficients[term.VariableIndex] = term.Coefficient;

            return new ArithmeticExpression(context, constant, coefficients);
        }

        public static ArithmeticExpression operator +(float constant, ArithmeticTerm term) {
            return term + constant;
        }

        public static ArithmeticExpression operator -(ArithmeticTerm term, float constant) {
            return term + -constant;
        }

        public static ArithmeticExpression operator -(float constant, ArithmeticTerm term) {
            return term + -constant;
        }

        public static ArithmeticTerm operator *(ArithmeticTerm term, float constant) {
            term.Coefficient *= constant;

            return term;
        }

        public static ArithmeticTerm operator *(float constant, ArithmeticTerm term) {
            return term * constant;
        }

        public static ArithmeticTerm operator /(ArithmeticTerm term, float constant) {
            return term * (1.0f / constant);
        }

        // Type Conversions

        public static implicit operator ArithmeticExpression(ArithmeticTerm term) {
            var coefficients = new float[term.Context.Variables.Count];

            coefficients[term.VariableIndex] = term.Coefficient;

            return new ArithmeticExpression(term.Context, coefficients);
        }
    }

    public struct ArithmeticVariable {
        public ArithmeticContext Context;

        public int Index;

        public float Value {
            get {
                for(var i = 0; i < Context.Variables.Count; i += 1) {
                    if(Context.SolutionVariableIndices[i] == Index) {
                        return Context.SolutionConstants[i];
                    }
                }

                return 0.0f;
            }
        }

        public ArithmeticVariable(ArithmeticContext context) {
            Context = context;

            Index = Context.Variables.Count;

            Context.Variables.Add(true);
        }

        // Variable-Variable operators

        public static ArithmeticExpression operator+(ArithmeticVariable a, ArithmeticVariable b) {
            var context = a.Context;

            var coefficients = new float[context.Variables.Count];

            coefficients[a.Index] = 1.0f;
            coefficients[b.Index] += 1.0f;

            return new ArithmeticExpression(context, coefficients);
        }

        public static ArithmeticTerm operator-(ArithmeticVariable variable) {
            return new ArithmeticTerm(variable.Context, variable.Index, -1.0f);
        }

        public static ArithmeticExpression operator-(ArithmeticVariable a, ArithmeticVariable b) {
            return a + -b;
        }

        public static ArithmeticConstraint operator==(ArithmeticVariable left, ArithmeticVariable right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = (ArithmeticExpression)right;

            return leftExpression == rightExpression;
        }

        public static ArithmeticConstraint operator!=(ArithmeticVariable left, ArithmeticVariable right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator<=(ArithmeticVariable left, ArithmeticVariable right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = (ArithmeticExpression)right;

            return leftExpression <= rightExpression;
        }

        public static ArithmeticConstraint operator>=(ArithmeticVariable left, ArithmeticVariable right) {
            return right <= left;
        }

        // Variable-float operators

        public static ArithmeticExpression operator+(ArithmeticVariable variable, float constant) {
            var coefficients = new float[variable.Context.Variables.Count];

            coefficients[variable.Index] = 1.0f;

            return new ArithmeticExpression(variable.Context, constant, coefficients);
        }

        public static ArithmeticExpression operator+(float constant, ArithmeticVariable variable) {
            return variable + constant;
        }

        public static ArithmeticExpression operator-(ArithmeticVariable variable, float constant) {
            return variable + -constant;
        }

        public static ArithmeticExpression operator-(float constant, ArithmeticVariable variable) {
            return constant + -variable;
        }

        public static ArithmeticTerm operator*(ArithmeticVariable variable, float coefficient) {
            return new ArithmeticTerm(variable.Context, variable.Index, coefficient);
        }

        public static ArithmeticTerm operator*(float coefficient, ArithmeticVariable variable) {
            return variable * coefficient;
        }

        public static ArithmeticTerm operator/(ArithmeticVariable variable, float coefficient) {
            return variable * (1.0f / coefficient);
        }

        public static ArithmeticConstraint operator==(ArithmeticVariable left, float right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = new ArithmeticExpression(left.Context, right);

            return leftExpression == rightExpression;
        }

        public static ArithmeticConstraint operator==(float left, ArithmeticVariable right) {
            return right == left;
        }

        public static ArithmeticConstraint operator!=(ArithmeticVariable left, float right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator!=(float left, ArithmeticVariable right) {
            throw new NotSupportedException();
        }

        public static ArithmeticConstraint operator<=(ArithmeticVariable left, float right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = new ArithmeticExpression(left.Context, right);

            return leftExpression <= rightExpression;
        }

        public static ArithmeticConstraint operator<=(float left, ArithmeticVariable right) {    
            return right >= left;
        }

        public static ArithmeticConstraint operator>=(ArithmeticVariable left, float right) {
            var leftExpression = (ArithmeticExpression)left;

            var rightExpression = new ArithmeticExpression(left.Context, right);

            return leftExpression >= rightExpression;
        }

        public static ArithmeticConstraint operator>=(float left, ArithmeticVariable right) {
            return right <= left;
        }

        // Type conversions

        public static implicit operator ArithmeticTerm(ArithmeticVariable variable) {
            return new ArithmeticTerm(variable.Context, variable.Index, 1.0f);
        }

        public static implicit operator ArithmeticExpression(ArithmeticVariable variable) {
            var coefficients = new float[variable.Context.Variables.Count];

            coefficients[variable.Index] = 1.0f;

            return new ArithmeticExpression(variable.Context, coefficients);
        }
    }
}

#pragma warning restore CS0660, CS0661