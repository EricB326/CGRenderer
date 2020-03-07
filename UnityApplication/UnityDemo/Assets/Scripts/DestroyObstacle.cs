using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestroyObstacle : MonoBehaviour
{
	[SerializeField] private GameObject m_destroyedCrate;

	private void OnCollisionEnter(Collision collision)
	{
		if (collision.gameObject.tag == "Stone")
		{
			Instantiate(m_destroyedCrate, transform.position, transform.rotation);
			Destroy(gameObject);
		}
	}
}
