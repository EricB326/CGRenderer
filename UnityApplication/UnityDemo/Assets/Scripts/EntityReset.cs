using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EntityReset : MonoBehaviour
{
	private void OnTriggerEnter(Collider other)
	{
		if (other.tag == "Stone")
			Destroy(other.gameObject);
		else if (other.tag == "Player")
			other.GetComponent<PlayerMovement>().ResetPlayerPos();
		else if (other.tag == "BrokenCrate")
			Destroy(other.gameObject);
	}
}
